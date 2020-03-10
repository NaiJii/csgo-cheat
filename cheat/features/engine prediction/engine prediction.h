#pragma once

#include "../../globals/globals.h"

namespace n_features {
	class c_engine_prediction {
	private:
		void* m_move_data;
		int* m_prediction_seed;
		int* m_prediction_player;
		float m_old_current_time;
		float m_old_frame_time;

	public:
		void run( n_sdk::C_BasePlayer* player, n_sdk::CUserCmd* cmd );
		void end( n_sdk::C_BasePlayer* player );
	};
}

extern n_features::c_engine_prediction g_engine_prediction;