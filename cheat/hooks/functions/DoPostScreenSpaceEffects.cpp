#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall DoPostScreenSpaceEffects( std::uintptr_t ecx, std::uintptr_t edx, const void* setup ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t, const void* ) >( DoPostScreenSpaceEffects );

			// sanity check
			if ( !n_globals::local_player )
				return original_fn( ecx, setup );

			// loop through all objects in the glow object manager
			for ( auto i = 0; i < n_interfaces::glow_object_manager->m_size; i++ ) {
				// sanity checks
				if ( n_interfaces::glow_object_manager->m_glow_object_definitions[ i ].is_unused( ) || !n_interfaces::glow_object_manager->m_glow_object_definitions[ i ].get_entity( ) )
					continue;

				// creating a variable on the stack for the current GlowObjectDefinition_t struct
				auto& glow_object = n_interfaces::glow_object_manager->m_glow_object_definitions[ i ];

				// creating a C_BaseEntity pointer on the stack
				n_sdk::C_BaseEntity* entity = glow_object.get_entity( );

				// sanity checks
				if ( !entity || !entity->is_player( ) || !entity->is_alive( ) || entity->is_in_same_team( n_globals::local_player ) )
					continue;

				// set the object to have some glow BABY
				glow_object.set( 1.f, 1.f, 1.f, 1.f, n_sdk::GLOW_STYLE_MODEL );
			}

			// returning the original virtual function
			return original_fn( ecx, setup );
		}
	}
}