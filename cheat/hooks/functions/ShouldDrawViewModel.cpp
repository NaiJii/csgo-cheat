#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall ShouldDrawViewModel( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( ShouldDrawViewModel );

			return original_fn( ecx );
		}
	}
}