#include "hooks.h"

namespace n_hooks {
	std::unique_ptr< n_vmt::c_vmt_manager > vmt_manager;
	WNDPROC original_window_procedure{ };

	bool setup( void ) {

		original_window_procedure = reinterpret_cast< WNDPROC >( SetWindowLongPtr( n_interfaces::app_window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( n_functions::WindowProc ) ) );
		vmt_manager = std::make_unique< n_vmt::c_vmt_manager >( );

		/* IBaseClientDLL */
		if ( !vmt_manager->hook_vft( n_interfaces::client_dll, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 6, n_functions::LevelInitPostEntity )
					&& vmt->hook( 7, n_functions::LevelShutdown )
					&& vmt->hook( 24, n_functions::WriteUsercmdDeltaToBuffer );
					// && vmt->hook( 37, n_functions::FrameStageNotify );
			} ) )
			return false;

		/* IClientMode */
		if ( !vmt_manager->hook_vft( n_interfaces::client_mode, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 17, n_functions::ShouldDrawFog ) 
					&& vmt->hook( 24, n_functions::CreateMove ) 
					&& vmt->hook( 27, n_functions::ShouldDrawViewModel )
					&& vmt->hook( 28, n_functions::ShouldDrawCrosshair ) 
					&& vmt->hook( 35, n_functions::GetViewModelFOV )
					&& vmt->hook( 44, n_functions::DoPostScreenSpaceEffects );
			} ) )
			return false;

		/* IClientEntityList */
		if ( !vmt_manager->hook_vft( n_interfaces::entity_list, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 11, n_functions::OnAddEntity )
					&& vmt->hook( 12, n_functions::OnRemoveEntity );
			} ) )
			return false;

		/* IStudioRender */
		if ( !vmt_manager->hook_vft( n_interfaces::studio_render, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 29, n_functions::DrawModel );
			} ) )
			return false;

		/* C_CSPlayer */
		if ( !vmt_manager->hook_vft( reinterpret_cast< void* >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) + 0x47 ), [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 177, n_functions::IsFollowingEntity )
					&& vmt->hook( 197, n_functions::DoExtraBoneProcessing );
			} ) )
			return false;

		/* C_BaseEntity */
		if ( !vmt_manager->hook_vft( reinterpret_cast< void* >( n_utilities::resolve_relative_call( n_utilities::find_pattern( "client_panorama.dll", "E8 ? ? ? ? 85 FF 75 AD 8B CB" ) ) + 0x9 ), [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return true;
			} ) )
			return false;

		/* IClientRenderable */
		if ( !vmt_manager->hook_vft( reinterpret_cast< void* >( n_utilities::find_pattern( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) + 0x4E ), [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 13, n_functions::SetupBones );
			} ) )
			return false;

		/* CPrediction */
		if ( !vmt_manager->hook_vft( n_interfaces::prediction, [ ]( n_vmt::c_vmt* vmt ) -> bool {
			return vmt->hook( 14, n_functions::InPrediction );
			} ) )
			return false;

		/* IVEngineClient */
		if ( !vmt_manager->hook_vft( n_interfaces::engine, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 27, n_functions::IsConnected )
					&& vmt->hook( 59, n_functions::FireEvents )
					&& vmt->hook( 93, n_functions::IsHltv );
			} ) )
			return false;

		/* IVEngineVGui */
		if ( !vmt_manager->hook_vft( n_interfaces::engine_vgui, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 14, n_functions::Paint );
			} ) )
			return false;

		/* ISurface */
		if ( !vmt_manager->hook_vft( n_interfaces::surface, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 15, n_functions::DrawSetColor )
					&& vmt->hook( 67, n_functions::LockCursor )
					&& vmt->hook( 116, n_functions::OnScreenSizeChanged );
			} ) )
			return false;
		
		/* IGameEventManager2 */
		if ( !vmt_manager->hook_vft( n_interfaces::game_event_manager, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 8, n_functions::FireEvent );
			} ) )
			return false;

		/* ISpatialQuery */
		if ( !vmt_manager->hook_vft( n_interfaces::engine->get_bsp_tree_query( ), [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 6, n_functions::ListLeavesInBox );
			} ) )
			return false;

		/* IMaterial */
		if ( !vmt_manager->hook_vft( reinterpret_cast< void* >( n_utilities::find_pattern( "materialsystem.dll", "C7 06 ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? 89 4D E8" ) + 0x2 ), [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 45, n_functions::GetColorModulation ) &&
                    vmt->hook( 44, n_functions::GetAlphaModulation );
			} ) )
			return false;

		/* IMaterialSystem */
		if ( !vmt_manager->hook_vft( n_interfaces::material_system, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 84, n_functions::GetMaterial );
			} ) )
			return false;

		/* IMDLCache */
		if ( !vmt_manager->hook_vft( n_interfaces::mdl_cache, [ ]( n_vmt::c_vmt* vmt ) -> bool {
				return vmt->hook( 10, n_functions::FindMDL );
			} ) )
			return false;

        /* SvCheats */
        if ( !vmt_manager->hook_vft( n_interfaces::cvar->find_var( "sv_cheats" ), []( n_vmt::c_vmt * vmt ) -> bool {
            return vmt->hook( 13, n_functions::SvCheatsGetBool );
        } ) )
            return false;

        /* Direct3D */
        if ( !vmt_manager->hook_vft( n_interfaces::direct3d, []( n_vmt::c_vmt * vmt ) -> bool { 
            return vmt->hook( 16, n_functions::Reset ) 
                && vmt->hook( 17, n_functions::Present );
        } ) )
            return false;

		return true;
	}

	bool unload( void ) {
        SetWindowLongPtr( n_interfaces::app_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( original_window_procedure ) );

		if ( !vmt_manager->unload( ) )
			return false;

		return true;
	}
}