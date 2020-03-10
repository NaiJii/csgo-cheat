#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall LevelShutdown( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t ) >( LevelShutdown );

			// call the original virtual function
			original_fn( ecx );

			// nullptr global variables
			n_globals::local_player = nullptr;
			n_globals::rtt = 0;
			n_globals::ping = 0;
		}
	}
}