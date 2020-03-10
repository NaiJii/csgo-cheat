#include "../hooks.h"

#include <windowsx.h>

#include "../../menu/menu.h"

namespace n_hooks {
	namespace n_functions {
		LRESULT __stdcall WindowProc( HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam ) {
			if ( msg == WM_MOUSEMOVE && g_menu.input( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) ) )
				return true;

            if ( g_menu.should_remove_input() )
                return true;

			return CallWindowProcA( original_window_procedure, hwnd, msg, wparam, lparam );		
		}
	}
}