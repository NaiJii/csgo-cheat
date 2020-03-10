#include "engine prediction.h"

n_features::c_engine_prediction g_engine_prediction;

namespace n_features {
	void c_engine_prediction::run( n_sdk::C_BasePlayer* player, n_sdk::CUserCmd* cmd ) {
		if ( !this->m_move_data )
			this->m_move_data = std::malloc( 182 );

		if ( !this->m_prediction_seed || !this->m_prediction_player ) {
			// could also get these from the runcommand vfunc
			this->m_prediction_seed = *reinterpret_cast< int** >( n_utilities::find_pattern( "client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86" ) + 0x1 );
			this->m_prediction_player = *reinterpret_cast< int** >( n_utilities::find_pattern( "client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2 );
		}

		// CPlayerMove::StartCommand
		{
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x3334 ) = reinterpret_cast< std::uintptr_t >( cmd );

			static auto unknown_fn = reinterpret_cast< void( __thiscall* )( int, int ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 8B 55 08 3B CA" ) );
			unknown_fn( reinterpret_cast< std::uintptr_t >( player + 0x3288 ), reinterpret_cast< std::uintptr_t >( cmd ) );

			*this->m_prediction_seed = cmd ? cmd->random_seed : -1;
			*this->m_prediction_player = reinterpret_cast< int >( player );
		}

		// backup globals
		this->m_old_current_time = n_interfaces::global_vars_base->current_time;
		this->m_old_frame_time = n_interfaces::global_vars_base->frame_time;

		// set globals appropriately
		n_interfaces::global_vars_base->current_time = player->get_tick_base( ) * n_interfaces::global_vars_base->interval_per_tick;
		n_interfaces::global_vars_base->frame_time = n_interfaces::prediction->m_engine_paused ? 0 : n_interfaces::global_vars_base->interval_per_tick;

		// backup prediction variables
		const bool old_in_prediction = n_interfaces::prediction->m_in_prediction;
		const bool old_is_first_time_prediction = n_interfaces::prediction->m_is_first_time_predicted;

		// setup prediction ( fix for double footstep )
		n_interfaces::prediction->m_in_prediction = false;
		n_interfaces::prediction->m_is_first_time_predicted = true;

		// set host
		n_interfaces::move_helper->set_host( player );

		// start track prediction erros
		n_interfaces::game_movement->start_track_prediction_errors( player );

		// do weapon selection
		if ( cmd->weapon_select != 0 ) {
			n_sdk::C_BaseCombatWeapon* weapon = reinterpret_cast< n_sdk::C_BaseCombatWeapon* >( n_interfaces::entity_list->get_client_entity( cmd->weapon_select ) );

			if ( weapon ) {
				n_sdk::CCSWeaponData* weapon_data = weapon->get_weapon_data( );

				if ( weapon_data )
					player->select_item( weapon_data->m_weapon_name, cmd->weapon_sub_type );
			}
		}

		n_sdk::C_BaseEntity* vehicle = nullptr;

		// check if the vehichle is valid
		if ( player->get_vehicle( ).is_valid( ) /*&& player->get_h_vehicle( ).get( ) != nullptr*/ )
			vehicle = reinterpret_cast< n_sdk::C_BaseEntity* >( n_interfaces::entity_list->get_client_entity_from_handle( player->get_vehicle( ) ) );

		if ( cmd->impulse && ( !vehicle || player->using_standard_weapons_in_vechile( ) ) )
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x31FC ) = cmd->impulse;

		// CBasePlayer::UpdateButtonState
		{
			cmd->buttons |= *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x3330 );

			int m_nButtons = cmd->buttons;
			int* m_afButtonLast = reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( player ) + 0x31F8 );
			int buttonsChanged = m_nButtons ^ *m_afButtonLast;
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x31EC ) = *m_afButtonLast;
			*m_afButtonLast = m_nButtons;
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x31F0 ) = m_nButtons & buttonsChanged;
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x31F4 ) = buttonsChanged & ~m_nButtons;
		}

		// check if player is standing on moving ground
		n_interfaces::prediction->check_moving_ground( player, n_interfaces::global_vars_base->frame_time );

		// copy from command to player
		player->set_local_view_angles( cmd->view_angles );

		// CPlayerMove::RunPreThink
		{
			if ( player->physics_run_think( 0 ) )
				player->pre_think( );
		}

		// CPlayerMove::RunThink
		{
			int* thinktick = reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( player ) + 0x0FC );
			if ( *thinktick != -1 && *thinktick > 0 && *thinktick <= player->get_tick_base( ) ) {
				*thinktick = -1;

				static auto unknown1_fn = reinterpret_cast< char( __thiscall* )( void*, int ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );
				unknown1_fn( player, 0 );

				player->think( );
			}
		}

		// setup input
		n_interfaces::prediction->setup_move( player, cmd, n_interfaces::move_helper, reinterpret_cast< n_sdk::CMoveData* >( this->m_move_data ) );

		if ( vehicle )
			n_utilities::get_virtual_function< void( __thiscall* )( int, n_sdk::C_BasePlayer*, void* ) >( vehicle, 5 )( reinterpret_cast< std::uintptr_t >( vehicle ), player, this->m_move_data );
		else
			// run movement
			n_interfaces::game_movement->process_movement( player, reinterpret_cast< n_sdk::CMoveData* >( this->m_move_data ) );

		// finish prediction
		n_interfaces::prediction->finish_move( player, cmd, reinterpret_cast< n_sdk::CMoveData* >( this->m_move_data ) );

		// invoke impact functions
		n_interfaces::move_helper->process_impacts( );

		// CPlayerMove::RunPostThink
		{
			auto post_think = [ ]( n_sdk::C_BasePlayer* player ) -> int {
				n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( n_interfaces::mdl_cache, 33 )( n_interfaces::mdl_cache );

				if ( player->is_alive( ) || *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x3A81 ) ) {
					n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( player, 339 )( player );

					if ( player->get_flags( ) & 1 )
						*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x3014 ) = 0;

					if ( *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x28BC ) == -1 )
						n_utilities::get_virtual_function< void( __thiscall* )( void*, int ) >( player, 218 )( player, 0 );

					n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( player, 219 )( player );

					static auto post_think_v_physics_fn = reinterpret_cast< bool( __thiscall* )( void* ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) );
					post_think_v_physics_fn( player );
				}

				static auto simulate_player_simulated_entities_fn = reinterpret_cast< bool( __thiscall* )( void* ) >( n_utilities::find_pattern( "client_panorama.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72" ) );
				simulate_player_simulated_entities_fn( player );

				return n_utilities::get_virtual_function< int( __thiscall* )( void* ) >( n_interfaces::mdl_cache, 34 )( n_interfaces::mdl_cache );
			};

			post_think( player );
		}

		// restore prediction variables
		n_interfaces::prediction->m_in_prediction = old_in_prediction;
		n_interfaces::prediction->m_is_first_time_predicted = old_is_first_time_prediction;
	}

	void c_engine_prediction::end( n_sdk::C_BasePlayer* player ) {
		// stop track prediction errors
		n_interfaces::game_movement->finish_track_prediction_errors( player );

		// rest the host
		n_interfaces::move_helper->set_host( nullptr );

		// reset game movement
		n_interfaces::game_movement->reset( );

		// CPlayerMove::FinishCommand
		{
			*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( player ) + 0x3334 ) = 0;

			*this->m_prediction_seed = -1;
			*this->m_prediction_player = 0;
		}

		if ( !n_interfaces::prediction->m_engine_paused )
			if ( n_interfaces::global_vars_base->frame_time > 0.f )
				player->get_tick_base( )++;

		// restore global variables
		n_interfaces::global_vars_base->current_time = this->m_old_current_time;
		n_interfaces::global_vars_base->frame_time = this->m_old_frame_time;
	}
}