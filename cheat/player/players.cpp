#include "players.h"
#include "../hooks/hooks.h"

namespace n_players {
	c_custom_player::c_custom_player( n_sdk::C_BaseEntity* entity ) {
		this->m_index = entity->networkable( )->ent_index( );
		this->m_player = reinterpret_cast< n_sdk::C_CSPlayer* >( entity );

		this->setup( );
	}

	void c_custom_player::setup( void ) {
		
	}

	void c_custom_player::unload( void ) {

	}
}