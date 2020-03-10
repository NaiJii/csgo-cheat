#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivmodelrender.h

namespace n_sdk {
	class IVModelRender {
	public:
		virtual_fn( is_forced_material_override( void ), 2, bool( __thiscall* )( void* ) );
	};
}