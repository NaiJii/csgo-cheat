#include "../hooks.h"
#include "../../menu/menu.h"
#include "../../render/render.h"

namespace n_hooks {
    namespace n_functions {
        HRESULT __stdcall Reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters ) {
            static auto original_fn = vmt_manager->get_original_fn<HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* )>( Reset );

            ////n_render::on_lost_device( );
            const auto result = original_fn( device, parameters );
            ////n_render::on_reset_device( device );

            return result;
        }
    }
}