#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/istudiorender.h

namespace n_sdk {
	class IMaterial;

	enum OverrideType_t {
		OVERRIDE_NORMAL = 0,
		OVERRIDE_BUILD_SHADOWS,
		OVERRIDE_DEPTH_WRITE,
		OVERRIDE_SSAO_DEPTH_WRITE,
	};

	class IStudioRender /*: public IAppSystem*/ {
	public:
		// sets the color/alpha modulation
		virtual_fn( set_color_modulation( float const* color ), 27, void( __thiscall* )( void*, float const* ), color );
		virtual_fn( set_alpha_modulation( float alpha ), 28, void( __thiscall* )( void*, float ), alpha );

		// causes a material to be used instead of the materials the model was compiled with
		virtual_fn( forced_material_override( IMaterial* new_material, OverrideType_t override_type = OVERRIDE_NORMAL, int overrides = 0 ), 33, void( __thiscall* )( void*, IMaterial*, OverrideType_t, int ), new_material, override_type, overrides );
	};
}