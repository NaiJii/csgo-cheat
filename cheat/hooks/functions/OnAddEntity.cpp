#include "../hooks.h"
#include "../../player/players.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall OnAddEntity( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IHandleEntity* entity, n_sdk::CBaseHandle handle ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, n_sdk::IHandleEntity*, n_sdk::CBaseHandle ) >( OnAddEntity );

			// casting the entity to IClientUnknown as done in the sdk
			n_sdk::IClientUnknown* unknown = reinterpret_cast< n_sdk::IClientUnknown* >( entity );

			// getting the base entity from the IClientUnknown pointer
			n_sdk::C_BaseEntity* base_entity = unknown->get_base_entity( );

			// add the player to the custom player array
			if ( base_entity && base_entity->is_player( ) )
				n_globals::players.push_back( n_players::c_custom_player( base_entity ) );

			// calling the original virtual function
			original_fn( ecx, entity, handle );
		}
	}
}