#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/tier1/convar.h

namespace n_sdk {
	class ConVar {
	public:
		// retrieve value
		virtual_fn( get_float( void ), 12, float( __thiscall* )( void* ) );
		virtual_fn( get_int( void ), 13, int( __thiscall* )( void* ) );

		// these just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
		virtual_fn( set_value( const char* value ), 14, void( __thiscall* )( void*, const char* ), value );
		virtual_fn( set_value( float value ), 15, void( __thiscall* )( void*, float ), value );
		virtual_fn( set_value( int value ), 16, void( __thiscall* )( void*, int ), value );

		char _pad0[ 0xC ];
		char* m_name;
	};
}