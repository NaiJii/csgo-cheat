#pragma once

#include <memory>
#include <vector>
#include <intrin.h>

#include "../vmt/vmt.h"
#include "../globals/globals.h"

namespace n_hooks {

	extern std::unique_ptr< n_vmt::c_vmt_manager > vmt_manager;
	extern WNDPROC original_window_procedure;

	// hook the vfts and use lamdas in conjunction with std::function to hook virtual functions
	bool setup( void );

	namespace n_functions {
		/* IBaseClientDLL */
		void __fastcall LevelInitPostEntity( std::uintptr_t ecx, std::uintptr_t edx ); // 6
		void __fastcall LevelShutdown( std::uintptr_t ecx, std::uintptr_t edx ); // 7
		bool __fastcall WriteUsercmdDeltaToBuffer( std::uintptr_t ecx, std::uintptr_t edx, int slot, void* buf, int from, int to, bool isnewcommand ); // 24
		void __fastcall FrameStageNotify( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::ClientFrameStage_t current_stage ); // 37

		/* IClientMode */
		bool __fastcall ShouldDrawFog( std::uintptr_t ecx, std::uintptr_t edx ); // 17
		bool __fastcall CreateMove( std::uintptr_t ecx, std::uintptr_t edx, float input_sample_time, n_sdk::CUserCmd* cmd ); // 24
		bool __fastcall ShouldDrawViewModel( std::uintptr_t ecx, std::uintptr_t edx ); // 27
		bool __fastcall ShouldDrawCrosshair( std::uintptr_t ecx, std::uintptr_t edx ); // 28
		float __fastcall GetViewModelFOV( std::uintptr_t ecx, std::uintptr_t edx ); // 35
		bool __fastcall DoPostScreenSpaceEffects( std::uintptr_t ecx, std::uintptr_t edx, const void* setup ); // 44

		/* IClientEntityList */
		void __fastcall OnAddEntity( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IHandleEntity* entity, n_sdk::CBaseHandle handle ); // 11
		void __fastcall OnRemoveEntity( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IHandleEntity* entity, n_sdk::CBaseHandle handle ); // 12

		/* IStudioRender */
		void __fastcall DrawModel( std::uintptr_t ecx, std::uintptr_t edx, void* results, void* info, void* bone_to_world, float* flex_weights, float* flex_delayed_weights, vec3_t& model_origin, int flags ); // 29

		/* C_CSPlayer */
		bool __fastcall IsFollowingEntity( std::uintptr_t ecx, std::uintptr_t edx ); // 177
		void __fastcall DoExtraBoneProcessing( n_sdk::C_CSPlayer* player, std::uintptr_t edx, void* hdr, vec3_t* pos, void* q, void* matrix, void* bone_list, void* context ); // 197

		/* C_BaseEntity */

		/* IClientRenderable */
		bool __fastcall SetupBones( std::uintptr_t ecx, std::uintptr_t edx, void* bone_to_world_out, int max_bones, int bone_mask, float current_time ); // 13

		/* CPrediction */
		bool __fastcall InPrediction( std::uintptr_t ecx, std::uintptr_t edx );

		/* IVEngineClient */
		bool __fastcall IsConnected( std::uintptr_t ecx, std::uintptr_t edx ); // 27
		void __fastcall FireEvents( n_sdk::IVEngineClient* engine, std::uintptr_t edx ); // xref: OnRenderStart
		bool __fastcall IsHltv( n_sdk::IVEngineClient* engine, std::uintptr_t edx ); // xref: ReEvaluateAnimLOD, SetupVelocity

		/* IVEngineVGui */
		void __fastcall Paint( std::uintptr_t ecx, std::uintptr_t edx, int mode ); // 14

		/* ISurface */
		void __fastcall DrawSetColor( n_sdk::ISurface* surface, std::uintptr_t edx, int r, int g, int b, int a ); // 15
		void __fastcall LockCursor( std::uintptr_t ecx, std::uintptr_t edx ); // 67
		void __fastcall OnScreenSizeChanged( std::uintptr_t ecx, std::uintptr_t edx, int old_width, int old_height ); // 116

		/* IGameEventManager2 */
		bool __fastcall FireEvent( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IGameEvent* event, bool done_broadcast ); // 8

		/* ISpatialQuery */
		int __fastcall ListLeavesInBox( std::uintptr_t ecx, std::uintptr_t edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max ); // 6

		/* IMaterial */
        float __fastcall GetAlphaModulation( n_sdk::IMaterial* ecx, std::uintptr_t edx ); // 44
		void __fastcall GetColorModulation( n_sdk::IMaterial* ecx, std::uintptr_t edx, float* r, float* g, float* b ); // 45

		/* IMaterialSystem */
		n_sdk::IMaterial* __fastcall GetMaterial( std::uintptr_t ecx, std::uintptr_t edx, const char* material_name, const char* texture_group_name, bool complain, const char* complain_prefix ); // 84

		/* IMDLCache */
		n_sdk::MDLHandle_t __fastcall FindMDL( std::uintptr_t ecx, std::uintptr_t edx, const char* relative_path ); // 10

        /* SvCheats */
        bool __fastcall SvCheatsGetBool( std::uintptr_t ecx, std::uintptr_t edx ); // 13

		/* Window Procedure */
		LRESULT __stdcall WindowProc( HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam );

	}

	// attempt to revert all of the vfts and free the allocated memory
	bool unload( void );
}