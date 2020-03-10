#include "../hooks.h"
#include "../../menu/menu.h"
#include "../../render/render.h"
#include "../../features/visuals/visuals.h"

namespace n_hooks {
    namespace n_functions {
        HRESULT __stdcall Present( IDirect3DDevice9* device, RECT* src, RECT* dst, HWND window_override, RGNDATA* region ) {
            //IDirect3DStateBlock9* state = nullptr;
            //IDirect3DVertexDeclaration9* vert_declaration = nullptr;
            //device->GetVertexDeclaration(&vert_declaration);
            //device->CreateStateBlock( D3DSBT_PIXELSTATE, &state );

            //state->Apply( );
            //state->Release( );
            //device->SetVertexDeclaration( vert_declaration );

            //n_render::draw_filled_rect( 25, 25, 100, 100, color_t( 255, 125, 0 ) );


            static auto original_fn = vmt_manager->get_original_fn<HRESULT(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*)>( Present );
            return original_fn( device, src, dst, window_override, region );
        }
    }
}