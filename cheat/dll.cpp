#include <windows.h>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "menu/menu.h"
#include "hooks/hooks.h"
#include "netvars/netvars.h"

bool unload_cheat( void ) {
#ifdef _DEBUG
	if ( !n_utilities::unload_console( ) )
		return false;
#endif

	if ( !n_hooks::unload( ) )
		return false;

	return true;
}

unsigned long __stdcall setup_cheat( const HMODULE dll ) {
	// try to setup all functions
	try {
#ifdef _DEBUG
		if ( !n_utilities::setup_console( "debug" ) )
			throw std::runtime_error( "failed to setup console" );
#endif
		if ( !n_interfaces::setup( ) )
			throw std::runtime_error( "failed to setup interfaces" );

		if ( !n_netvars::setup( ) )
			throw std::runtime_error( "failed to setup netvars" );

		if ( !n_hooks::setup( ) )
			throw std::runtime_error( "failed to setup hooks" );

		if ( !g_menu.get( ) )
			throw std::runtime_error( "failed to setup menu" );

	} catch ( const std::runtime_error& exception ) {
		// compiler warnings smh
		UNREFERENCED_PARAMETER( exception );

#ifdef _DEBUG
		// provide enough time to read the error message
		std::this_thread::sleep_for( std::chrono::seconds( 2 ) );

		// display a message box with some info about the exception
		MessageBoxA( 0, exception.what( ), "debug", MB_OK );
#endif
	}

	// inifinite loop to keep the thread alive until we want to uninject
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

	// attempt to unload the cheat
	if ( !unload_cheat( ) )
		return 0;

	// decrements the reference count of the dll and then calls ExitThread to terminate the thread
	FreeLibraryAndExitThread( dll, 0 );

	return 1;
}

// https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain
int __stdcall DllMain( const HMODULE dll, const DWORD reason, const LPVOID reserved ) {
	// disables the DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications for the specified dynamic-link library ( dll ).
	DisableThreadLibraryCalls( dll );

	// if the dll is attaching to the virtual address space of the current process
	if ( reason == DLL_PROCESS_ATTACH ) {
		// create a thread for the cheat setup
		const auto cheat_thread = CreateThread( 0, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( setup_cheat ), dll, 0, 0 );

		// if CreateThread fails it will return 0
		if ( !cheat_thread )
			return 0;

		// its importatnt to call CloseHandle as handles occupy kernal and userspace memory ( which is limited )
		CloseHandle( cheat_thread );
	}

	return 1;
}