#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall IsConnected( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( IsConnected );

			// .text:103C8AA0 84 C0                             test    al, al <-- sig this to get the unknown return
			// .text:103C8AA2 75 04                             jnz     short loc_103C8AA8 
			// .text:103C8AA4 B0 01                             mov     al, 1

			static auto unknown_return = n_utilities::find_pattern( "client_panorama.dll", "84 C0 75 04 B0 01 5F" );

			// allows us to always access the inventory
			if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) == unknown_return )
				return false;

			return original_fn(ecx);
		}
	}
}