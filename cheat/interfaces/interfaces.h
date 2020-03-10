#pragma once

#include <d3d9.h>

#include "../sdk.h"
#include "../utilities/utilities.h"

namespace n_interfaces {
	extern HWND app_window;
	extern n_sdk::IBaseClientDLL* client_dll;
	extern n_sdk::IClientMode* client_mode;
	extern n_sdk::IMDLCache* mdl_cache;
	extern n_sdk::IClientEntityList* entity_list;
	extern n_sdk::IGameEventManager2* game_event_manager;
	extern n_sdk::CClientState* client_state;
	extern n_sdk::IVEngineClient* engine;
	extern n_sdk::IStudioRender* studio_render;
	extern n_sdk::IMaterialSystem* material_system;
	extern n_sdk::IVModelRender* model_render;
	extern n_sdk::IVModelInfo* model_info;
	extern n_sdk::IClientLeafSystem* leaf_system;
	extern n_sdk::IWeaponSystem* weapon_system;
	extern n_sdk::CGlowObjectManager* glow_object_manager;
    extern n_sdk::CInput* input;
	extern n_sdk::CGlobalVarsBase* global_vars_base;
	extern n_sdk::ISurface* surface;
	extern n_sdk::IVEngineVGui* engine_vgui;
	extern n_sdk::IVDebugOverlay* debug_overlay;
	extern n_sdk::ILocalize* localize;
	extern n_sdk::ICvar* cvar;
	extern n_sdk::CPrediction* prediction;
	extern n_sdk::CGameMovement* game_movement;
	extern n_sdk::IMoveHelper* move_helper;
    extern IDirect3DDevice9* direct3d;

	bool setup( void );
}