#pragma once

#include "../math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/gamemovement.h

namespace n_sdk {
	class C_BasePlayer;
	class CMoveData;

	class CGameMovement {
	public:
		virtual	~CGameMovement( void ) = 0;
		virtual void process_movement( C_BasePlayer* player, CMoveData* move ) = 0;
		virtual void reset( void ) = 0;
		virtual void start_track_prediction_errors( C_BasePlayer* player ) = 0;
		virtual void finish_track_prediction_errors( C_BasePlayer* player ) = 0;
		virtual void diff_print( char const* fmt, ... ) = 0;
		virtual vec3_t const& get_player_mins( bool ducked ) const = 0;
		virtual vec3_t const& get_player_maxs( bool ducked ) const = 0;
		virtual vec3_t const& get_player_view_offset( bool ducked ) const = 0;
		virtual bool is_moving_player_stuck( void ) const = 0;
		virtual C_BasePlayer* get_moving_player( void ) const = 0;
		virtual void unblock_pusher( C_BasePlayer* player, C_BasePlayer* pusher ) = 0;
		virtual void setup_movement_bounds( CMoveData* move ) = 0;
	};
}