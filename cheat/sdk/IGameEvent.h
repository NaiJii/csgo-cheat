#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/igameevents.h

namespace n_sdk {
	class IGameEvent {
	public:
		virtual ~IGameEvent( void ) { };

		// get event name
		virtual const char* get_name( void ) const = 0;

		// if event handled reliable
		virtual bool is_reliable( void ) const = 0;

		// if event is never networked
		virtual bool is_local( void ) const = 0;

		// check if data field exists
		virtual bool is_empty( const char* key_name = 0 ) = 0;

		// data access
		virtual bool get_bool( const char* key_name = 0, bool default_value = false ) = 0;
		virtual int get_int( const char* key_name = 0, int default_value = 0 ) = 0;
		virtual unsigned long long get_uint_64( char const* key_name = 0, unsigned long long default_value = 0 ) = 0;
		virtual float get_float( const char* key_name = 0, float default_value = 0.0f ) = 0;
		virtual const char* get_string( const char* key_name = 0, const char* default_value = "" ) = 0;
		virtual const wchar_t* get_wstring( char const* key_name = 0, const wchar_t* default_value = L"" ) = 0;
	};
}