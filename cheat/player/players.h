#pragma once

#include "../entity/entity.h"

namespace n_players {
	class c_custom_player {
	public:		
		c_custom_player( n_sdk::C_BaseEntity* entity );

		// used for setting up hooking, called in initialiser
		void setup( void );

		// used in destruction of this object
		void unload( void );

		// class variables
		int m_index;
		n_sdk::C_CSPlayer* m_player;
	};
}