#include "misc.h"

n_features::c_misc g_misc;

namespace n_features {
	void c_misc::bunny_hop( void ) {
		if ( this->m_move_type == n_sdk::MOVETYPE_NOCLIP
			|| this->m_move_type == n_sdk::MOVETYPE_LADDER )
			return;

		if ( ( n_globals::user_cmd->buttons & n_sdk::IN_JUMP ) && ( this->m_flags & n_sdk::FL_ONGROUND ) )
			n_globals::user_cmd->buttons &= ~n_sdk::IN_JUMP;
	}

	void c_misc::fake_lag( void ) {
		//*n_globals::send_packet = n_interfaces::client_state->m_chocked_commands > 15;	
	}

	void c_misc::run( void ) {
		// set up class variables
		this->m_move_type = n_globals::local_player->get_movetype( );
		this->m_flags = n_globals::local_player->get_flags( );

		// run bhop
		this->bunny_hop( );

		// run fake lag
		this->fake_lag( );
	}
}