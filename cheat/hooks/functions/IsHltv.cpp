#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall IsHltv( n_sdk::IVEngineClient* engine, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( void* ) >( IsHltv );
			
			// current return address
			const auto current_return_address = std::uintptr_t( _ReturnAddress( ) );

			// valid fuck off addresses
			static auto ReEvaluateAnimLOD_return_address = n_utilities::find_pattern( "client_panorama.dll", "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7" ); // n_utilities::resolve_relative_call( n_utilities::find_pattern( "client_panorama.dll", "E8 ? ? ? ? 8B CE E8 ? ? ? ? 8B 8F ? ? ? ? 83" ) ) + 0x2D;
			static auto SetupVelocity_return_address = n_utilities::find_pattern( "client_panorama.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ?" );
			static auto AccumulateLayers_return_address = n_utilities::find_pattern( "client_panorama.dll", "84 C0 75 0D F6 87" ); // pvs fix noob

			if ( current_return_address == ReEvaluateAnimLOD_return_address
				|| current_return_address == SetupVelocity_return_address
				|| current_return_address == AccumulateLayers_return_address )
				return true;

			return original_fn( engine );
		}
	}
}