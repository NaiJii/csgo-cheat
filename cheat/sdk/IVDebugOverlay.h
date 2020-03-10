#pragma once

#include "../utilities/utilities.h"
#include "../math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivdebugoverlay.h

namespace n_sdk {
	class IVDebugOverlay {
	public:
		virtual_fn( screen_position( const vec3_t& point, vec3_t& screen ), 13, int( __thiscall* )( void*, const vec3_t&, vec3_t& ), point, screen );
	};
}