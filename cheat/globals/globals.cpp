#include "globals.h"

#include "../player/players.h"
#include "../features/notifications/notifications.h"

namespace n_globals {
	n_sdk::C_CSPlayer* local_player = nullptr;
	n_sdk::CUserCmd* user_cmd = nullptr;

	bool* send_packet = nullptr;
	int rtt{ };
	int ping{ };
	float frame_rate{ };
	int tick_rate{ };

	std::vector< n_players::c_custom_player > players{ };
	std::deque< n_features::c_notification > notifications{ };
}