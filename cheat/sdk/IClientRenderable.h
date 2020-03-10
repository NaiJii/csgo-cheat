#pragma once

#include "../utilities/utilities.h"
#include "../math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iclientrenderable.h

namespace n_sdk {
	class IClientUnknown;

	class IClientRenderable {
	public:
		// gets at the containing class...
		virtual_fn( get_client_unknown( void ), 0, IClientUnknown* ( __thiscall* )( void* ) );

		// returns the bounds relative to the origin (render bounds)
		virtual_fn( get_render_bounds( vec3_t& mins, vec3_t& maxs ), 17, void( __thiscall* )( void*, vec3_t&, vec3_t& ), mins, maxs );
	};
}