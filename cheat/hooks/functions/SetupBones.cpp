#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall SetupBones( std::uintptr_t ecx, std::uintptr_t edx, void* bone_to_world_out, int max_bones, int bone_mask, float current_time ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t, void*, int, int, float ) >( SetupBones );

			// entity is always 0x4 behind IClientRenderable pointer so we can just
			n_sdk::C_BaseEntity* entity = reinterpret_cast< n_sdk::C_BaseEntity* >( ecx - 0x4 );

			// sanity check
			if ( !entity )
				return original_fn( ecx, bone_to_world_out, max_bones, bone_mask, current_time );

			// value we are going to change
			int* value = reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( entity ) + 0x0A68 );

			// backup value
			const int old_value = *value;

			// force ShouldSkipAnimationFrame to return false
			*value = 0;

			// get the original virtual function at this point
			const auto original = original_fn( ecx, bone_to_world_out, max_bones, bone_mask, current_time );

			// restore the value
			*value = old_value;

			// return
			return original;
		}
	}
}