#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		n_sdk::IMaterial* __fastcall GetMaterial( std::uintptr_t ecx, std::uintptr_t edx, const char* material_name, const char* texture_group_name, bool complain, const char* complain_prefix ) {
			static auto original_fn = vmt_manager->get_original_fn< n_sdk::IMaterial* ( __thiscall* )( std::uintptr_t, const char*, const char*, bool, const char* ) >( GetMaterial );

			// if the material is the blurry shit
			if ( std::strcmp( material_name, "dev/scope_bluroverlay" ) == 0 )
				// replace the material with "nothing"
				return original_fn( ecx, "dev/clearalpha", nullptr, complain, complain_prefix );

			// returning the original virtual function
			return original_fn( ecx, material_name, texture_group_name, complain, complain_prefix );
		}
	}
}