#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall DoExtraBoneProcessing( n_sdk::C_CSPlayer* player, std::uintptr_t edx, void* hdr, vec3_t* pos, void* q, void* matrix, void* bone_list, void* context ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( n_sdk::C_CSPlayer*, void*, vec3_t*, void*, void*, void*, void* ) >( DoExtraBoneProcessing );

			// getting CCSGOPlayerAnimState
			const auto animation_state = player->get_animation_state( );

			// sanity check
			if ( !animation_state ) 
				return original_fn( player, hdr, pos, q, matrix, bone_list, context );

			// backup the value we are going to change
			const bool old_on_ground = animation_state->m_on_ground;

			// prevent DoProceduralFootPlant from running
			animation_state->m_on_ground = false;

			// call the original virtual function 
			original_fn( player, hdr, pos, q, matrix, bone_list, context );

			// restore the value
			animation_state->m_on_ground = old_on_ground;
		}
	}
}