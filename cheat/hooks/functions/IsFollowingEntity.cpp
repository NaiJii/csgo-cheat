#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall IsFollowingEntity( std::uintptr_t ecx, std::uintptr_t ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( IsFollowingEntity );

			// current return address
			const auto current_return_address = std::uintptr_t( _ReturnAddress( ) );

			// valid fuck off addresses v-1
			static auto target_return_address = n_utilities::resolve_relative_call( n_utilities::find_pattern( "client_panorama.dll", "E8 ? ? ? ? 83 7D FC 00 8B F8" ) ) + 0x1A;

			// if the current return addres is THE TARGET RETURN ADDRESS WE GOT WHAT WE WANTED
			if ( current_return_address == target_return_address )
				return true;

			return original_fn( ecx );
		}
	}
}