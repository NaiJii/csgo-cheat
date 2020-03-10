#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall ShouldDrawCrosshair( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( ShouldDrawCrosshair );

			// sanity checks
			if ( !n_globals::local_player || !n_globals::local_player->is_alive( ) )
				return original_fn( ecx );

			// getting the active cs weapon
			n_sdk::C_WeaponCSBase* weapon = n_globals::local_player->get_active_cs_weapon( );

			// sanity check
			if ( !weapon )
				return original_fn( ecx );

			// getting the weapon data of the active weapon
			n_sdk::CCSWeaponData* weapon_data = weapon->get_weapon_data( );

			// sanity check
			if ( !weapon_data )
				return original_fn( ecx );

			// backup weapon type
			const int old_weapon_type = weapon_data->m_weapon_type;

			// set the weapon type check to a non valid id
			weapon_data->m_weapon_type = 0xFF;

			// call the draw crosshair function
			weapon->draw_crosshair( );

			//restore the weapon type
			weapon_data->m_weapon_type = old_weapon_type;

			// return false because "we dont want to draw a crosshair"
			return false;
		}
	}
}