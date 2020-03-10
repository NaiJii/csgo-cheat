#include "../hooks.h"

#include "../../menu/menu.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall OnScreenSizeChanged( std::uintptr_t ecx, std::uintptr_t edx, int old_width, int old_height ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, int, int ) >( OnScreenSizeChanged );

			// call the original virtual function
			original_fn( ecx, old_width, old_height );

			// need to re-"get" the fonts and screen size in the ISurface render wrapper on screen size change
			const auto result = g_menu.m_render.get( );

			// need to do something if it fails tbh
		}
	}
}