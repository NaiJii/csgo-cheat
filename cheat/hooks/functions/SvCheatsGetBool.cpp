#include "../hooks.h"

namespace n_hooks {
    namespace n_functions {
        bool __fastcall SvCheatsGetBool( std::uintptr_t ecx, std::uintptr_t edx ) {
            static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t ) >( SvCheatsGetBool );
            if ( !original_fn )
                return false;

            const auto current_return_address = std::uintptr_t( _ReturnAddress() );

            static auto CamThink_return_adress = n_utilities::find_pattern( "client_panorama.dll", "85 C0 75 30 38 86" );
            static auto grenade_preview_return_adress = n_utilities::find_pattern( "client_panorama.dll", "8B 0D ? ? ? ? F3 0F 7E 00 8B 40 08 89 44 24 24" );

            if ( current_return_address == CamThink_return_adress || current_return_address == grenade_preview_return_adress )
                return true;

            return original_fn( ecx );
        }
    }
}