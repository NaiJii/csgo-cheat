#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/tier1/ilocalize.h

namespace n_sdk {
	class ILocalize {
	public:
		// finds the localized text for tokenName
		virtual_fn( find( char const* token_name ), 11, wchar_t* ( __thiscall* )( void*, char const* ), token_name );
	};
}