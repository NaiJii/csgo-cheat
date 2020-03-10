#include "../../hooks/hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall GetColorModulation( n_sdk::IMaterial* ecx, std::uintptr_t edx, float* r, float* g, float* b ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( n_sdk::IMaterial*, float*, float*, float* ) >( GetColorModulation );

			// call the original virtual function
			original_fn( ecx, r, g, b );

			// get the group of the material
			const auto material_group = ecx->get_texture_group_name( );

			// only modulate certain materials
			if ( std::strcmp( material_group, "World textures" ) != 0
				&& std::strcmp( material_group, "StaticProp textures" ) != 0
				&& std::strcmp( material_group, "SkyBox textures" ) != 0 )
				return;

			// need to find a good method to enable us to modulate static props

			// modulate the materials
			*r *= 0.2f;
			*g *= 0.2f;
			*b *= 0.2f;
		}
	}
}