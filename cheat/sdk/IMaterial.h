#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/materialsystem/imaterial.h

namespace n_sdk {
	enum MaterialVarFlags_t {
		MATERIAL_VAR_DEBUG = ( 1 << 0 ),
		MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
		MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
		MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),

		MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
		MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
		MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
		MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
		MATERIAL_VAR_ALPHATEST = ( 1 << 8 ),
		// MATERIAL_VAR_UNUSED = (1 << 9),
		MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
		MATERIAL_VAR_MODEL = ( 1 << 11 ),
		MATERIAL_VAR_FLAT = ( 1 << 12 ),
		MATERIAL_VAR_NOCULL = ( 1 << 13 ),
		MATERIAL_VAR_NOFOG = ( 1 << 14 ),
		MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
		MATERIAL_VAR_DECAL = ( 1 << 16 ),
		MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ),
		// MATERIAL_VAR_UNUSED = (1 << 18),
		MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ),
		MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
		MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
		MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
		MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ),
		MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
		MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ),
		MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
		MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
		MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
		MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
		MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
		MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
	};

	class IMaterial {
	public:
		// get the name of the material.  This is a full path to 
		// the vmt file starting from "hl2/materials" (or equivalent) without
		// a file extension. 
		virtual_fn( get_name( void ), 0, const char* ( __thiscall* )( void* ) );
		virtual_fn( get_texture_group_name( void ), 1, const char* ( __thiscall* )( void* ) );

		// material var flags...
		virtual_fn( set_material_var_flag( MaterialVarFlags_t flag, bool on ), 29, void( __thiscall* )( void*, MaterialVarFlags_t, bool ), flag, on ); 

        // if the material is wrong 
        virtual_fn( is_error_material( void ), 42, bool( __thiscall* )( void* ) );

		// gets the current alpha modulation
		virtual_fn( get_alpha_modulation( void ), 44, void( __thiscall* )( void* ) );
		virtual_fn( get_color_modulation( float* r, float* g, float* b ), 45, void( __thiscall* )( void*, float*, float*, float* ), r, g, b );
	};
}