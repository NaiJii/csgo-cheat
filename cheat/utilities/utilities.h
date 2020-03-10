#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <psapi.h>

namespace n_utilities {
	/* console */
	bool setup_console( const std::string& window_name );
	void log_console( const std::string& string );
	bool unload_console( void );

	/* pattern scanner */
	std::uintptr_t find_pattern( const std::string& module_name, const std::string& pattern );

	/* used for virtual function rtti */
	template< typename t > int protect_full_memory_region( t adr, int protection ) {
		// cast user input to LPCVOID for VirtualQuery
		const auto address = LPCVOID( adr );

		// query memory region's info
		_MEMORY_BASIC_INFORMATION mi{ };
		VirtualQuery( address, &mi, sizeof( mi ) );

		// do what the function does wew
		DWORD old_protection;
		VirtualProtect( mi.BaseAddress, mi.RegionSize, protection, &old_protection );

		// returns old region protection
		return old_protection;
	}

	/* resolves e8 relative jumps */
	template< typename c, typename t = std::uintptr_t > t resolve_relative_call( c address ) {
		// calculate the relative address
		const auto relative_address = *reinterpret_cast< std::uintptr_t* >( std::uintptr_t( address ) + 0x1 );

		// calling address
		return ( t )( std::uintptr_t( address ) + 0x5 + relative_address );
	}

	/* interface */
	template< typename t > t get_interface( const std::string& module_name, const std::string& interface_name ) {
		// retrieves the module handle for the specified module
		const auto module_handle = GetModuleHandleA( module_name.c_str( ) );

		// if GetModuleHandleA fails it will return 0
		if ( !module_handle )
			return t( );

		// creates a function that allows us to get the address of the module
		const auto create_interface_fn = reinterpret_cast< t( * )( const char*, int* ) >( GetProcAddress( module_handle, "CreateInterface" ) );

		// if GetProcAddress fails it will return 0
		if ( !create_interface_fn )
			return t( );

		// getting the interfaces address
		auto interface_address = create_interface_fn( interface_name.c_str( ), nullptr );

		// if GetProcAddress fails it will return 0
		if ( !interface_address )
			return t( );

#ifdef _DEBUG
		// print to the console that we successfully found the interface
		log_console( "found " + interface_name );
#endif

		// if everything succeeds, return the address of the interface
		return interface_address;
	}

	/* virtual function */
	template < typename t > static t get_virtual_function( void* base, const std::uintptr_t index ) {
		return ( *reinterpret_cast< t** >( base ) )[ index ];
	}
}

#define in_range( x, a, b )	\
	( x >= a && x <= b ) 

#define get_bits( x ) \
	( in_range( x, '0', '9' ) ? ( x - '0' ) : ( (x & ( ~0x20 ) ) - 'A' + 0xA ) )

#define get_byte( x ) \
	( get_bits( x[0] ) << 4 | get_bits( x[1] ) )

#define virtual_fn( function_name, index, type, ... ) \
	auto function_name { \
		return n_utilities::get_virtual_function< type >( this, index )( this, __VA_ARGS__ ); \
	};