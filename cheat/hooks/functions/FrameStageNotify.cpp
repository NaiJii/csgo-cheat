#include "../hooks.h"

#include "../../features/visuals/visuals.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall FrameStageNotify( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::ClientFrameStage_t current_stage ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, n_sdk::ClientFrameStage_t ) >( FrameStageNotify );

			// call the original virtual function
			original_fn( ecx, current_stage );
		}
	}
}