#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		float __fastcall GetViewModelFOV( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< float( __thiscall* )( std::uintptr_t ) >( GetViewModelFOV );

			return 100.f;
		}
	}
}