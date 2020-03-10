#include "../../hooks/hooks.h"

namespace n_hooks {
    namespace n_functions {
        float __fastcall GetAlphaModulation( n_sdk::IMaterial* ecx, std::uintptr_t edx ) {
            static auto original_fn = vmt_manager->get_original_fn< float( __thiscall* )( n_sdk::IMaterial* ) >( GetAlphaModulation );

            // get the material passed by the function right now
            if ( !ecx || ecx->is_error_material() )
                return original_fn( ecx );

            // get the group of the material
            const auto material_group = ecx->get_texture_group_name();

            // only modulate certain materials
            if ( std::strcmp( material_group, "World textures" ) != 0 && std::strcmp( material_group, "StaticProp textures" ) != 0 )
                return original_fn( ecx );

            return 0.7f;
        }
    }
}