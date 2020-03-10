#include "../hooks.h"

#include "../../player/players.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall OnRemoveEntity( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IHandleEntity* entity, n_sdk::CBaseHandle handle ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, n_sdk::IHandleEntity*, n_sdk::CBaseHandle ) >( OnRemoveEntity );

			// casting the entity to IClientUnknown as done in the sdk
			n_sdk::IClientUnknown* unknown = reinterpret_cast< n_sdk::IClientUnknown* >( entity );

			// getting the base entity from the IClientUnknown pointer
			n_sdk::C_BaseEntity* base_entity = unknown->get_base_entity( );

			// if the entity is sane
			if ( base_entity ) {
				// find if the removed entity was in our vector
				const auto iterator = std::find_if( n_globals::players.begin( ), n_globals::players.end( ), [ & ]( const n_players::c_custom_player& player ) {
					return player.m_index == base_entity->networkable( )->ent_index( );
					} );

				// if the iterator is valid
				if ( n_globals::players.end( ) != iterator ) {
					// unload the player class instance
					n_globals::players.at( std::uintptr_t( iterator - n_globals::players.begin( ) ) ).unload( );

					// remove it from the list
					n_globals::players.erase( iterator );
				}
			}

			// calling the original virtual function
			original_fn( ecx, entity, handle );
		}
	}
}