#include "../hooks.h"

#include "../../menu/menu.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall DrawSetColor( n_sdk::ISurface* surface, std::uintptr_t edx, int r, int g, int b, int a ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( n_sdk::ISurface*, int, int, int, int ) >( DrawSetColor );

			// current return address
			const auto current_return_address = std::uintptr_t( _ReturnAddress( ) );

			// valid fuck off addresses v2
			static auto scope_arc_return_address = n_utilities::find_pattern( "client_panorama.dll", "8B 0D ? ? ? ? FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 7C 24 1C" ) ;
			static auto scope_lens_return_address = n_utilities::find_pattern( "client_panorama.dll", "8B 4C 24 20 8B 54 24 1C C7" );

			// if the scope arc or scope lens is been drawn
			if ( current_return_address == scope_arc_return_address || current_return_address == scope_lens_return_address )
				// we dont want these things to draw so
				return original_fn( surface, r, g, b, 0 );

			// valid fuck off addresses v3
			static auto scope_lines_clear_return_address = n_utilities::find_pattern( "client_panorama.dll", "8B 0D ? ? ? ? 83 FE 01 7F" );
			static auto scope_lines_blurry_return_address = n_utilities::find_pattern( "client_panorama.dll", "8B 0D ? ? ? ? FF B7 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 4C 24 24" );

			// if the scope lines are being cleared
			if ( current_return_address != scope_lines_clear_return_address && current_return_address != scope_lines_blurry_return_address )
				// we only want to fuck with the things above so
				return original_fn( surface, r, g, b, a );

			// draw our own scope
			// handy that we are in DrawSetColor isnt it? lets set the color
			original_fn( surface, 0, 0, 0, 255 );
			// get the screen size
			const int x = g_menu.m_render.m_screen_size.x, y = g_menu.m_render.m_screen_size.y;
			// draw the lines brudha
			surface->draw_line( x / 2, 0, x / 2, y ); // i know multiplication is faster than division but then i would have to cast everything to an int to avoid compiler warnings
			surface->draw_line( 0, y / 2, x, y / 2 );

			// return the original virtual function but set the alpha to 0 so the orignal scope lines arent drawn
			return original_fn( surface, r, g, b, 0 );
		}
	}
}