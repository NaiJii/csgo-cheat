#include "../hooks.h"

#include "../../menu/menu.h"
#include "../../features/visuals/visuals.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall Paint( std::uintptr_t ecx, std::uintptr_t edx, int mode ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, int ) >( Paint );

			// call the original virtual function
			original_fn( ecx, mode );

			// disable post processinng
			static auto override_post_processing_disable = *reinterpret_cast< bool** >( n_utilities::find_pattern( "client_panorama.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85" ) + 0x2 );
			*override_post_processing_disable = true;

			// sanity check
			if ( mode & n_sdk::PAINT_UIPANELS ) {
				// xref: "-pixel_offset_x" => sig the sub
				static auto start_drawing_fn = reinterpret_cast< void( __thiscall* )( void* ) >( n_utilities::find_pattern( "vguimatsurface.dll", "55 8B EC 83 E4 C0 83 EC 38" ) );

				// xref: "-pixel_offset_y" => sig the sub directly below
				static auto finish_drawing_fn = reinterpret_cast< void( __thiscall* )( void* ) >( n_utilities::find_pattern( "vguimatsurface.dll", "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );

				start_drawing_fn( n_interfaces::surface ); {
					// draw the visuals first because we want the menu to overlay them
					g_visuals.paint( );

					// draw the menu
					g_menu.paint( );

				} finish_drawing_fn( n_interfaces::surface );
			}
		}
	}
}