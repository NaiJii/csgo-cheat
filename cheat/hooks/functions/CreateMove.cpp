#include "../hooks.h"

#include "../../features/engine prediction/engine prediction.h"
#include "../../features/misc/misc.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall CreateMove( std::uintptr_t ecx, std::uintptr_t edx, float input_sample_time, n_sdk::CUserCmd* cmd ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t, float, n_sdk::CUserCmd* ) >( CreateMove );

            // sanity checks
            if ( !cmd || !cmd->command_number )
                return false;

            if ( original_fn( ecx, input_sample_time, cmd ) )
                n_interfaces::engine->set_view_angles( &cmd->view_angles );

			// setting global
			n_globals::user_cmd = cmd;

			// getting the previous stackframe
			const auto ebp = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) - sizeof( std::uintptr_t* ) );
			// creating a pointer to send_packet ( REMEMBER to DEREFERENCE the pointer when modifing )
			n_globals::send_packet = reinterpret_cast< bool* >( ebp - 0x1C );

            if ( !n_globals::local_player ) {
                n_globals::local_player = n_sdk::C_CSPlayer::get_local_player();
                if ( !n_globals::local_player )
                    return false;
            }

			// sanity checks
			if ( !n_globals::local_player || !n_globals::local_player->is_alive( ) )
				return original_fn( ecx, input_sample_time, cmd );

			// getting the INetChannelInfo pointer
			n_sdk::INetChannelInfo* channel_info = n_interfaces::engine->get_net_channel_info( );

			// sanity check
			if ( channel_info ) {
				// calculate the latency of the user
				const float latency = channel_info->get_latency( 1 /* FLOW_INCOMING */ ) + channel_info->get_latency( 0 /* FLOW_OUTGOING */ );

				// setting globals
				n_globals::rtt = static_cast< int >( latency * 1000.f );
				n_globals::ping = static_cast< int >( channel_info->get_average_latency( 0 /* FLOW_OUTGOING */ ) * 1000.f );
			}

			// setting another global
			n_globals::tick_rate = static_cast< int >( 1.f / n_interfaces::global_vars_base->interval_per_tick );

			// run prediction
			g_engine_prediction.run( n_globals::local_player, n_globals::user_cmd ); {
				// run misc
				g_misc.run( );

			} g_engine_prediction.end( n_globals::local_player );

            cmd->buttons |= n_sdk::IN_BULLRUSH;

            cmd->view_angles.clamp();
            cmd->forward_move = std::clamp( cmd->forward_move, -450.f, 450.f );
            cmd->side_move = std::clamp( cmd->side_move, -450.f, 450.f );
            cmd->up_move = std::clamp( cmd->up_move, -320.f, 320.f );
            return false;
		}
	}
}