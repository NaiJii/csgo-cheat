#include "interfaces.h"

#include <thread>
#include <chrono>

#include "../utilities/utilities.h"

namespace n_interfaces {
	HWND app_window = NULL;
	n_sdk::IBaseClientDLL* client_dll = nullptr;
	n_sdk::IClientMode* client_mode = nullptr;
	n_sdk::IMDLCache* mdl_cache = nullptr;
	n_sdk::IClientEntityList* entity_list = nullptr;
	n_sdk::IGameEventManager2* game_event_manager = nullptr;
	n_sdk::CClientState* client_state = nullptr;
	n_sdk::IVEngineClient* engine = nullptr;
	n_sdk::IStudioRender* studio_render = nullptr;
	n_sdk::IMaterialSystem* material_system = nullptr;
	n_sdk::IVModelRender* model_render = nullptr;
	n_sdk::IVModelInfo* model_info = nullptr;
	n_sdk::IClientLeafSystem* leaf_system = nullptr;
	n_sdk::IWeaponSystem* weapon_system = nullptr;
	n_sdk::CGlowObjectManager* glow_object_manager = nullptr;
	n_sdk::CGlobalVarsBase* global_vars_base = nullptr;
    n_sdk::CInput* input = nullptr;
	n_sdk::ISurface* surface = nullptr;
	n_sdk::IVEngineVGui* engine_vgui = nullptr;
	n_sdk::IVDebugOverlay* debug_overlay = nullptr;
	n_sdk::ILocalize* localize = nullptr;
	n_sdk::ICvar* cvar = nullptr;
	n_sdk::CPrediction* prediction = nullptr;
	n_sdk::CGameMovement* game_movement = nullptr;
	n_sdk::IMoveHelper* move_helper = nullptr;

	// attempts to get all the interfaces. the function successfully gets all the interfaces it will return true
	bool setup( void ) {
		// wait for the window
		while ( !( app_window = FindWindowA( "Valve001", nullptr ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

		client_dll = n_utilities::get_interface< n_sdk::IBaseClientDLL* >( "client_panorama.dll", "VClient018" );

		if ( !client_dll )
			return false;

		client_mode = **reinterpret_cast< n_sdk::IClientMode*** >( ( *reinterpret_cast< std::uintptr_t** > ( client_dll ) )[ 10 ] + 0x5 );

		if ( !client_mode )
			return false;

		mdl_cache = n_utilities::get_interface< n_sdk::IMDLCache* >( "datacache.dll", "MDLCache004" );

		if ( !mdl_cache )
			return false;

		entity_list = n_utilities::get_interface< n_sdk::IClientEntityList* >( "client_panorama.dll", "VClientEntityList003" );

		if ( !entity_list )
			return false;

		game_event_manager = n_utilities::get_interface< n_sdk::IGameEventManager2* >( "engine.dll", "GAMEEVENTSMANAGER002" );

		if ( !game_event_manager )
			return false;

		engine = n_utilities::get_interface< n_sdk::IVEngineClient* >( "engine.dll", "VEngineClient014" );

		if ( !engine )
			return false;

		client_state = **reinterpret_cast< n_sdk::CClientState*** >( ( *reinterpret_cast< std::uintptr_t** > ( engine ) )[ 12 ] + 0x10 );

		if ( !client_state )
			return false;

		studio_render = n_utilities::get_interface< n_sdk::IStudioRender* >( "studiorender.dll", "VStudioRender026" );

		if ( !studio_render )
			return false;

		material_system = n_utilities::get_interface< n_sdk::IMaterialSystem* >( "materialsystem.dll", "VMaterialSystem080" );

		if ( !material_system )
			return false;

		model_render = n_utilities::get_interface< n_sdk::IVModelRender* >( "engine.dll", "VEngineModel016" );

		if ( !model_render )
			return false;

		model_info = n_utilities::get_interface< n_sdk::IVModelInfo* >( "engine.dll", "VModelInfoClient004" );

		if ( !model_info )
			return false;

		leaf_system = n_utilities::get_interface< n_sdk::IClientLeafSystem* >( "client_panorama.dll", "ClientLeafSystem002" );

		if ( !leaf_system )
			return false;

		weapon_system = *reinterpret_cast< n_sdk::IWeaponSystem** >( n_utilities::find_pattern( "client_panorama.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 0x2 );

		if ( !weapon_system )
			return false;

		glow_object_manager = reinterpret_cast< n_sdk::CGlowObjectManager* ( __cdecl* )( void ) >( n_utilities::find_pattern( "client_panorama.dll", "A1 ? ? ? ? A8 01 75 4B 0F 57 C0" ) )( );

		if ( !glow_object_manager )
			return false;

        input = *reinterpret_cast<n_sdk::CInput * *>( n_utilities::find_pattern( "client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 0x1 );

        if ( !input )
            return false;

		global_vars_base = **reinterpret_cast< n_sdk::CGlobalVarsBase*** >( ( *reinterpret_cast< std::uintptr_t** >( client_dll ) )[ 11 ] + 10 );

		if ( !global_vars_base )
			return false;

		surface = n_utilities::get_interface< n_sdk::ISurface* >( "vguimatsurface.dll", "VGUI_Surface031" );

		if ( !surface )
			return false;

		engine_vgui = n_utilities::get_interface< n_sdk::IVEngineVGui* >( "engine.dll", "VEngineVGui001" );

		if ( !engine_vgui )
			return false;

		debug_overlay = n_utilities::get_interface< n_sdk::IVDebugOverlay* >( "engine.dll", "VDebugOverlay004" );

		if ( !debug_overlay )
			return false;

		localize = n_utilities::get_interface< n_sdk::ILocalize* >( "localize.dll", "Localize_001" );

		if ( !localize )
			return false;

		cvar = n_utilities::get_interface< n_sdk::ICvar* >( "vstdlib.dll", "VEngineCvar007" );

		if ( !cvar )
			return false;

		prediction = n_utilities::get_interface< n_sdk::CPrediction* >( "client_panorama.dll", "VClientPrediction001" );

		if ( !prediction )
			return false;

		game_movement = n_utilities::get_interface< n_sdk::CGameMovement* >( "client_panorama.dll", "GameMovement001" );

		if ( !game_movement )
			return false;

		move_helper = **reinterpret_cast< n_sdk::IMoveHelper*** >( n_utilities::find_pattern( "client_panorama.dll", "8B 0D ? ? ? ? 8B 45 FC 51 8B" ) + 0x2 );

		if ( !move_helper )
			return false;

		return true;
	}
}