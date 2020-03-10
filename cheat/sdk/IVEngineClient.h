#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

namespace n_sdk {
	class INetChannelInfo;

	typedef struct player_info_s {
		char _pad0[ 0x8 ]; // 0
		// network xuid
		int m_xuid_low; // 8
		int m_xuid_high; // 12
		// scoreboard information
		char m_name[ 256 ]; // 16
		char _pad1[ 0x13C ]; // 272
	} player_info_t; // 588

	class IVEngineClient {
	public:
		// gets the dimensions of the game window
		virtual_fn( get_screen_size( int& width, int& height ), 5, void( __thiscall* )( void*, int&, int& ), width, height );

		// fill in the player info structure for the specified player index (name, model, etc.)
		virtual_fn( get_player_info( int ent_num, player_info_t* info ), 8, bool( __thiscall* )( void*, int, player_info_t* ), ent_num, info );

		// retrieve the player entity number for a specified userID
		virtual_fn( get_player_for_user_id( int user_id ), 9, bool( __thiscall* )( void*, int ), user_id );

		// get the entity index of the local player
		virtual_fn( get_local_player( void ), 12, int( __thiscall* )( void* ) );

        // set local view angles
        virtual_fn( set_view_angles( angle_t* view_angles ), 19, void( __thiscall* )( void*, angle_t* ), view_angles );

        // returns true if the player is active in game
        virtual_fn( is_in_game( void ), 26, bool( __thiscall* )( void* ) );

		// returns true if the player is connected, but not necessarily active in game (could still be loading)
		virtual_fn( is_connected( void ), 27, bool( __thiscall* )( void* ) );

		// gets a way to perform spatial queries on the BSP tree
		virtual_fn( get_bsp_tree_query( void ), 43, void* ( __thiscall* )( void* ) );

		// returns info interface for client netchannel
		virtual_fn( get_net_channel_info( void ), 78, INetChannelInfo* ( __thiscall* )( void* ) );
	};
}