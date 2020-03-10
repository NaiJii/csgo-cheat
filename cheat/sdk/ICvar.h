#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icvar.h

namespace n_sdk {
	class ConVar;

	class ICvar {
	public:
		// try to find the cvar pointer by name
		virtual_fn( find_var( const char* var_name ), 15, ConVar* ( __thiscall* )( void*, const char* ), var_name );
	};
}