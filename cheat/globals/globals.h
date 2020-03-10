#pragma once

#include <vector>
#include <deque>

#include "../entity/entity.h"

// prototype bc if we include it here and then wanna use globals in the player class we r fucked
namespace n_players {
	class c_custom_player;
}

namespace n_features {
	class c_notification;
}

// a namespace to store variable that are used freqeuntly and / or need to be accessed through different .cpp files
namespace n_globals {
	/* pointers */
	extern n_sdk::C_CSPlayer* local_player;
	extern n_sdk::CUserCmd* user_cmd;
	
	/* normal shit */
	extern bool* send_packet;
	extern int rtt;
	extern int ping;
	extern float frame_rate;
	extern int tick_rate;
	
	/* storages */
	extern std::vector< n_players::c_custom_player > players;
	extern std::deque< n_features::c_notification > notifications;
}