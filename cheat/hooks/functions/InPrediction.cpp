#include "../../hooks/hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall InPrediction( std::uintptr_t ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( InPrediction );
			
			// current return address
			const auto current_return_address = std::uintptr_t( _ReturnAddress( ) );

			// valid fuck off addresses v69
			static auto MaintainSequenceTransitions_return_address = n_utilities::find_pattern( "client_panorama.dll", "84 C0 74 17 8B 87" );

			// prevent MaintainSequenceTransitions from running ( stop decay? ) https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/client/c_baseanimating.cpp#L1578
			if ( current_return_address == MaintainSequenceTransitions_return_address )
				return true;

			// return and call the original virtual function
			return original_fn( ecx );
		}
	}
}