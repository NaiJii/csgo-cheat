#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall LevelInitPostEntity( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t ) >( LevelInitPostEntity );

			// call the original virtual function
			original_fn( ecx );

			// set some globals
			n_globals::local_player = n_sdk::C_CSPlayer::get_local_player( );
		}
	}
}