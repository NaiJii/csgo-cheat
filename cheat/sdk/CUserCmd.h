#pragma once

#include "../math/vector3d.h"
#include "../math/qangle.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h

namespace n_sdk {
	class CUserCmd {
	public:
		char _pad0[ 0x4 ];
		// for matching server and client commands for debugging
		int command_number;
		// the tick the client created this command
		int tick_count;
		// player instantaneous view angles.
		angle_t view_angles;
		vec3_t aim_direction;
		// forward velocity.
		float forward_move;
		// sideways velocity.
		float side_move;
		// upward velocity.
		float up_move;
		// attack button states
		int buttons;
		// impulse command issued.
		int impulse;
		// current weapon id
		int weapon_select;
		int weapon_sub_type;
		// for shared random functions
		int random_seed;
		// mouse accum in x from create move
		short moused_x;
		// mouse accum in y from create move
		short moused_y;
		// client only, tracks whether we've predicted this command at least once
		bool has_been_predicted;
	};
}