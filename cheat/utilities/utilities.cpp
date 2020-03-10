#include "utilities.h"

#include <chrono>
#include <iomanip>

namespace n_utilities {
	bool setup_console( const std::string& window_name ) {
		// allocates a new console for the calling process
		if ( !AllocConsole( ) )
			return false;

		// re-open the standard steam stdin
		const auto in = freopen_s( reinterpret_cast< FILE** >( stdin ), "CONIN$", "r", stdin );

		// if freopen_s succeeds it will return 0
		if ( in != 0 )
			return false;

		// re-open the standard steam stdout
		const auto out = freopen_s( reinterpret_cast< FILE** >( stdout ), "CONOUT$", "w", stdout );

		// if freopen_s succeeds it will return 0
		if ( out != 0 )
			return false;

		// sets the current title for the console window
		if ( !SetConsoleTitleA( window_name.c_str( ) ) )
			return false;

		// if nothing fails then return true
		return true;
	}

	void log_console( const std::string& string ) {
		// lambda function to get the current system time
		auto get_system_time = [ ]( std::tm& time_info ) -> void {
			// gets the current system time
			const auto system_now = std::chrono::system_clock::now( );

			// convert the system time to __time64_t
			const auto now = std::chrono::system_clock::to_time_t( system_now );

			// converts the time into calender time
			localtime_s( &time_info, &now );
		};

		// decalre a variable to store the current time
		tm current_time{ };
		// get the current time
		get_system_time( current_time );
		
		// convert the calender time into a string and print the log string
		std::cout << "[" << std::put_time( &current_time, "%T" ) << "] " << string << std::endl;
	}

	bool unload_console( void ) {
		// close the standard stream stdin 
		const auto in = std::fclose( reinterpret_cast< FILE* >( stdin ) );

		// if fclose succeeds it will return 0
		if ( in != 0 )
			return false;

		// close the standard stream stdout 
		const auto out = std::fclose( reinterpret_cast< FILE* >( stdout ) );

		// if fclose succeeds it will return 0
		if ( out != 0 )
			return false;

		// detach the calling process from the console
		if ( !FreeConsole( ) )
			return false;

		// if nothing fails then return true
		return true;
	}

	std::uintptr_t find_pattern( const std::string& module_name, const std::string& pattern ) {
		// convert an ida-style signature into a byte sequence
		static auto pattern_to_byte = [ ]( const char* pattern ) {
			// stack variable prerequisits 
			auto bytes = std::vector< int >{ };
			auto start = const_cast< char* >( pattern );
			auto end = const_cast< char* >( pattern ) + std::strlen( pattern );

			// convert the pattern into the corresponding bytes
			for ( auto current = start; current < end; ++current ) {
				// if the current byte is a wildcard
				if ( *current == '?' ) {
					// ignore that byte
					++current;

					// check if the follwing byte is also a wildcard
					if ( *current == '?' )
						// ignore that byte
						++current;

					// dummy byte
					bytes.push_back( -1 );
				} else {
					// convert the character to a byte on a hex base
					bytes.push_back( std::strtoul( current, &current, 16 ) );
				}
			}

			// return the determined amount of bytes from the ida-style pattern
			return bytes;
		};

		// retrieves the module handle for the specified module
		const auto module_handle = GetModuleHandleA( module_name.c_str( ) );

		// if GetModuleHandleA fails it will return 0
		if ( !module_handle )
			return 0;

		// creating a structure to retrieve the module information
		MODULEINFO module_info{ 0 };

		// if K32GetModuleInformation fails it will return 0
		if ( !K32GetModuleInformation( GetCurrentProcess( ), module_handle, &module_info, sizeof( MODULEINFO ) ) )
			return 0;

		// get the size of the module - this is the area that we will search for the byte sequence
		const auto image_size = module_info.SizeOfImage;

		// sanity check
		if ( !image_size )
			return 0;

		// create a variable on the stack to store the converted byte sequence
		auto pattern_bytes = pattern_to_byte( pattern.c_str( ) );

		// get the number of byes in the module
		const auto image_bytes = reinterpret_cast< byte* >( module_handle );

		// two stack variable prerequisits 
		const auto signature_size = pattern_bytes.size( );
		const auto signature_bytes = pattern_bytes.data( );

		// loop through all bytes in the module and check if the converted byte sequence matches
		for ( auto i{ 0ul }; i < image_size - signature_size; ++i ) {
			// create a variable to determine the success of the search
			auto byte_sequence_found = true;

			// iterate over all the bytes from the pattern and check if it matches
			for ( auto j{ 0ul }; j < signature_size; ++j ) {
				// the bytes dont match and the byte is not a wildcard
				if ( image_bytes[ i + j ] != signature_bytes[ j ] && signature_bytes[ j ] != -1 ) {
					// we did not find a sequence 
					byte_sequence_found = false;
					// as never a byte was matched or a wildcard, the pattern is invalid so we should discontinue the search
					break;
				}
			}

			// we found the desired pattern
			if ( byte_sequence_found )
				// we can return the address of the pattern
				return reinterpret_cast< std::uintptr_t >( &image_bytes[ i ] );
		}

		// pattern was not found within the module
		return 0;
	}
}