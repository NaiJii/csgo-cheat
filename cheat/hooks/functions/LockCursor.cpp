#include "../hooks.h"

#include "../../menu/menu.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall LockCursor( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t ) >( LockCursor );

			// check if the menu is open
			if ( g_menu.is_opened( ) ) {
				// unlock the cursor
				n_interfaces::surface->unlock_cursor( );
				// return so the rest of the function isnt called
				return;
			}

			// call the original virtual function
			original_fn( ecx );
		}
	}
}