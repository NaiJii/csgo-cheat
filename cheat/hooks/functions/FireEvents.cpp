#include "../../hooks/hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall FireEvents( n_sdk::IVEngineClient* ecx, std::uintptr_t edx ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( n_sdk::IVEngineClient* ) >( FireEvents );

			// cl.isactive
			if ( n_interfaces::client_state->m_sign_on_state == 6 /* SIGNONSTATE_FULL */ )
				// dont need unnecessary checks
				for ( auto event_info = n_interfaces::client_state->m_events; event_info != nullptr; event_info = event_info->m_next_object )
					// setting the dlay to zero so there is no event delay
					event_info->m_delay = 0.f;

			// returning the original virtual function
			return original_fn( ecx );
		}
	}
}