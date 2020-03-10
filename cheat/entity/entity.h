#pragma once

#include "../utilities/utilities.h"
#include "../interfaces/interfaces.h"
#include "../netvars/netvars.h"
#include "../math/math.h"

namespace n_sdk {
	class C_BaseEntity {
	public:
		IClientRenderable* renderable( void ) {
			return reinterpret_cast< IClientRenderable* >( reinterpret_cast< std::uintptr_t >( this ) + 0x4 );
		}

		IClientNetworkable* networkable( void ) {
			return reinterpret_cast< IClientNetworkable* >( reinterpret_cast< std::uintptr_t >( this ) + 0x8 );
		}

		netvar_fn( get_movetype( void ), int, "DT_BaseEntity", "movetype" );

		offset_fn( get_coordinate_frame( void ), matrix3x4_t, 0x444 ); // could get via C_BaseEntity::EntityToWorldTransform

		virtual_fn( get_abs_origin( void ), 10, vec3_t& ( __thiscall* )( void* ) ); // unk
		virtual_fn( get_team_number( void ), 87, int( __thiscall* )( void* ) ); // unk - find it in ClientModeCSNormal::FireGameEvent
		virtual_fn( is_alive( void ), 155, bool( __thiscall* )( void* ) ); // unk
		virtual_fn( is_player( void ), 157, bool( __thiscall* )( void* ) ); // unk

		bool is_in_same_team( C_BaseEntity* entity ) {
			if ( !entity )
				return false;

			return entity->get_team_number( ) == this->get_team_number( );
		}
	};

	class C_BaseCombatCharacter : public C_BaseEntity {
	public:
		netvar_fn( get_active_weapon_handle( void ), CBaseHandle, "DT_BaseCombatCharacter", "m_hActiveWeapon" );
	};

	class C_BasePlayer : public C_BaseCombatCharacter {
	public:
		netvar_fn( get_vehicle( void ), CBaseHandle, "DT_BasePlayer", "m_hVehicle" );
		netvar_fn( get_health( void ), int, "DT_BasePlayer", "m_iHealth" );
		netvar_fn( get_flags( void ), int, "DT_BasePlayer", "m_fFlags" );

		offset_fn( get_tick_base( void ), int, 0x342C );

		virtual_fn( think( void ), 138, void( __thiscall* )( void* ) ); // 8B C1 8B 50 40
		virtual_fn( pre_think( void ), 317, void( __thiscall* )( void* ) ); // 55 8B EC 83 E4 F8 51 56 8B F1 8B 06
		virtual_fn( select_item( const char* string, int sub_type = 0 ), 329, void( __thiscall* )( void*, const char*, int ), string, sub_type ); // 55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06
		virtual_fn( set_local_view_angles( angle_t& angle ), 372, void( __thiscall* )( void*, angle_t& ), angle ); // unk

		bool using_standard_weapons_in_vechile( void ) {
			static auto using_standard_weapons_in_vechile_fn = reinterpret_cast< bool( __thiscall* )( void* ) >( n_utilities::find_pattern( "client_panorama.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41" ) );
			return using_standard_weapons_in_vechile_fn( this );
		}

		bool physics_run_think( int index ) {
			static auto physics_run_think_fn = reinterpret_cast< bool( __thiscall* )( void*, int ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );
			return physics_run_think_fn( this, index );
		}
	};

	class C_BaseAnimating : public C_BasePlayer {
	public:
		int get_bone_id( const std::string& name ) {
			static auto lookup_bone_fn = reinterpret_cast< int( __thiscall* )( void*, const char* ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75" ) );
			return lookup_bone_fn( this, name.c_str( ) );
		}

		vec3_t get_bone_position( const int bone_id ) {
			vec3_t position{ }, rotation{ };

			static auto get_bone_position_fn = reinterpret_cast< void( __thiscall* )( void*, int, vec3_t*, vec3_t* ) >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 30 8D" ) );
			get_bone_position_fn( this, bone_id, &position, &rotation );

			return position;
		}
	};

	class C_EnvTonemapController : public C_BaseEntity {
	public:

	};

	class C_BaseCSGrenade : public C_BaseEntity {
	public:

	};

	class C_BaseCombatWeapon : public C_BaseCSGrenade {
	public:
		offset_fn( get_item_definition_index( void ), int, 0x2FAA );

		CCSWeaponData* get_weapon_data( void ) {
			return n_interfaces::weapon_system->get_weapon_data( this->get_item_definition_index( ) );
		}
	};

	class C_WeaponCSBase : public C_BaseCombatWeapon {
	public:
		virtual_fn( draw_crosshair( void ), 403, void( __thiscall* )( void* ) ); // 55 8B EC 83 E4 F0 83 EC 38 56 8B F1 8B 0D ? ? ? ? 57
		virtual_fn( get_weapon_type( void ), 453, int( __thiscall* )( void* ) ); // 8B 01 FF 90 ? ? ? ? 8B 80 ? ? ? ? C3 CC 56
	};

	class C_BaseAttributableItem : public C_BaseCombatWeapon {
	public:

	};

	class C_CSPlayer : public C_BaseAnimating {
	public:
		offset_fn( get_animation_state( void ), CCSGOPlayerAnimState*, 0x3900 );

		static C_CSPlayer* get_local_player( void ) {
			return reinterpret_cast< C_CSPlayer* >( n_interfaces::entity_list->get_client_entity( n_interfaces::engine->get_local_player( ) ) );
		}

		C_BaseCombatWeapon* get_active_weapon( void ) {
			return reinterpret_cast< C_BaseCombatWeapon* >( n_interfaces::entity_list->get_client_entity_from_handle( this->get_active_weapon_handle( ) ) );
		}

		C_WeaponCSBase* get_active_cs_weapon( void ) {
			return reinterpret_cast< C_WeaponCSBase* >( this->get_active_weapon( ) );
		}
	};
}