#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		n_sdk::MDLHandle_t __fastcall FindMDL( std::uintptr_t ecx, std::uintptr_t edx, const char* relative_path ) {
			static auto original_fn = vmt_manager->get_original_fn< n_sdk::MDLHandle_t( __thiscall* )( std::uintptr_t, const char* ) >( FindMDL );

			return original_fn( ecx, relative_path );
		}
	}
}