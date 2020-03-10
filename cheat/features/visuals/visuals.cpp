#include "visuals.h"

#include "../../player/players.h"
#include "../../menu/menu.h"
#include "../notifications/notifications.h"
#include "../../fmt/format.h"

n_features::c_visuals g_visuals{ };

namespace n_features {
	bool c_visuals::calculate_box( n_sdk::C_BaseEntity* entity ) {
		// getting render bounds
		vec3_t min{ }, max{ };
		entity->renderable( )->get_render_bounds( min, max );

		// creating some variables on the stack to store information
		vec3_t screen_origin_position{ }, origin_position = entity->get_abs_origin( ) - vec3_t( 0.f, 0.f, 10.f );
		vec3_t screen_top_position{ }, top_position = origin_position + vec3_t( 0.f, 0.f, max.z + 10.f );

		// attempt to get the world cords to screen cords
		if ( !g_menu.m_render.world_to_screen( origin_position, screen_origin_position )
			|| !g_menu.m_render.world_to_screen( top_position, screen_top_position ) )
			return false;

		// definitions
		const float height = screen_origin_position.y - screen_top_position.y;
		const float width = height / 2;

		// altering the box argument dimensions
		this->m_box.x = static_cast< int >( screen_origin_position.x - width * 0.5 );
		this->m_box.y = static_cast< int >( screen_top_position.y );
		this->m_box.w = static_cast< int >( width );
		this->m_box.h = static_cast< int >( height );

		// success!
		return true;
	}

	void c_visuals::draw_watermark( void ) {
		// create a string on the stack
		std::string info = "cheat | ";

		// add the fps to the string
		n_globals::frame_rate = static_cast< float >( 0.9 * n_globals::frame_rate + ( 1.0 - 0.9 ) * n_interfaces::global_vars_base->absolute_frame_time );
		info.append( "fps: " + fmt::format_int( static_cast< int >( 1 / n_globals::frame_rate ) ).str( ) + " | " );

		// adding rtt to the string
		info.append( "rtt: " + fmt::format_int( n_globals::rtt ).str( ) + "ms | " );

		// adding rate ( tick rate ) to the string
		info.append( "rate: " + fmt::format_int( n_globals::tick_rate ).str( ) + " | " );

		// adding player count to the string
		info.append( "players: " + fmt::format_int( n_globals::players.size( ) ).str( ) );

		// adapted function from menu.cpp
		auto draw_backplate = [ ]( const int x, const int y, const std::pair< int, int >& pair, const int x_offset, const int y_offset ) {
			const constexpr int size_of_grey_area = 2;

			auto buffer = size_of_grey_area + 3;

			g_menu.m_render.outlined_rect( x - pair.first - buffer, y - buffer, pair.first + buffer + buffer + x_offset, pair.second + buffer + buffer + y_offset, color_t( 30, 30, 30, 255 ) );

			buffer--;

			g_menu.m_render.outlined_rect( x - pair.first - buffer, y - buffer, pair.first + buffer + buffer + x_offset, pair.second + buffer + buffer + y_offset, color_t( 110, 110, 110, 255 ) );

			for ( int i = 0; i < size_of_grey_area; i++ ) {
				buffer--;

				g_menu.m_render.outlined_rect( x - pair.first - buffer, y - buffer, pair.first + buffer + buffer + x_offset, pair.second + buffer + buffer + y_offset, color_t( 70, 70, 70, 255 ) );
			}

			buffer--;

			g_menu.m_render.outlined_rect( x - pair.first - buffer, y - buffer, pair.first + buffer + buffer + x_offset, pair.second + buffer + buffer + y_offset, color_t( 110, 110, 110, 255 ) );

			g_menu.m_render.filled_rect( x - pair.first, y, pair.first + x_offset, pair.second + y_offset, color_t( 30, 30, 30, 255 ) );
		};

		// draw the back plate AND yes ik the std::pair will have 0, 0 values for the first frame but idc
		draw_backplate( this->m_info.x - 4, this->m_info.y - 1, this->m_info.size, 8, 3 );

		// draw the info
		this->m_info.x = g_menu.m_render.m_screen_size.x - 10, this->m_info.y = 10;
		this->m_info.size = g_menu.m_render.text( g_menu.m_render.m_fonts.tahoma, info, this->m_info.x, this->m_info.y, n_render::c_graphics::RIGHT, color_t( 255, 255, 255, 220 ) );
	}

	void c_visuals::draw_notifications( void ) {
		// looping through the deque
		for ( std::uintptr_t i = 0; i < n_globals::notifications.size( ); i++ ) {
			// creating a variable on the stack for the notification
			auto& notification = n_globals::notifications.at( i );

			// sanity check
			if ( n_interfaces::global_vars_base->current_time - notification.m_spawn_time > notification.m_config.life_span ) {
				// remove the notification from the deque
				n_globals::notifications.erase( n_globals::notifications.begin( ) + i );
				// move on to the next notification
				continue;
			}

			// drawing the notification ( here we are using the text size of the m_info struct because it will be the same height )
			g_menu.m_render.text( g_menu.m_render.m_fonts.tahoma, notification.m_message, notification.m_config.x, notification.m_config.y + this->m_info.y * i, false, color_t( 255, 255, 255, 220 ) );
		}
	}

	void c_visuals::draw_server_hitboxes( void ) { // ye this can just stay here i guess
		// the call to DrawServerHitboxes
		static auto DrawServerHitboxes_call = n_utilities::find_pattern( "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );

		// didnt know where to put this tbh
		auto util_player_by_index = [ ]( const int index ) -> void* {
			static auto util_player_by_index_fn = reinterpret_cast< void* ( __thiscall* )( int ) >( n_utilities::find_pattern( "server.dll", "85 C9 7E 2A A1" ) );
			return util_player_by_index_fn( index );
		};

		// getting the entity pointer via UTIL_PlayerByIndex
		void* player = util_player_by_index( n_interfaces::engine->get_local_player( ) );

		// sanity check
		if ( player ) {
			// how long the hitboxes should stay
			const float duration = -1.f;

			// oopsie
			__asm {
				pushad		
				movss xmm1, duration
				push 0
				mov ecx, player
				call DrawServerHitboxes_call
				popad
			}
		}
	}

	void c_visuals::draw_box( const int r, const int g, const int b, const int a ) {
		// outline rectangles
		g_menu.m_render.outlined_rect( this->m_box.x - 1, this->m_box.y - 1, this->m_box.w + 2, this->m_box.h + 2, color_t( 10, 10, 10, a ) );
		g_menu.m_render.outlined_rect( this->m_box.x + 1, this->m_box.y + 2, this->m_box.w - 2, this->m_box.h - 2, color_t( 10, 10, 10, a ) );
		// main rectangle
		g_menu.m_render.outlined_rect( this->m_box.x, this->m_box.y, this->m_box.w, this->m_box.h, color_t( r, g, b, a ) );
	}

	void c_visuals::draw_name( n_sdk::C_CSPlayer* player, const int r, const int g, const int b, const int a ) {
		// creating a player_info_t varaible on the stack
		n_sdk::player_info_t info{ };

		// getting the player info
		if ( !n_interfaces::engine->get_player_info( player->networkable( )->ent_index( ), &info ) )
			return;

		// the player name
		std::string name = info.m_name;

		// we like lower case out here
		std::transform( name.begin( ), name.end( ), name.begin( ), std::tolower );

		// if the player is a bot
		if ( !info.m_xuid_low )
			name.insert( 0, "BOT " );

		// draw the players name
		g_menu.m_render.text( g_menu.m_render.m_fonts.tahoma, name, this->m_box.x + this->m_box.w / 2, this->m_box.y, n_render::c_graphics::CENTERED_X | n_render::c_graphics::BOTTOM_Y, color_t( r, g, b, a ) );
	}

	void c_visuals::draw_weapon( n_sdk::C_CSPlayer* player, const int r, const int g, const int b, const int a ) {
		// getting the active weapon
		n_sdk::C_BaseCombatWeapon* weapon = player->get_active_weapon( );

		// sanity check
		if ( !weapon )
			return;

		// getting the weapon data of the active weapon
		n_sdk::CCSWeaponData* weapon_data = weapon->get_weapon_data( );

		// sanity check
		if ( !weapon_data )
			return;

		// getting the weapon name
		const std::string name = weapon_data->m_hud_name;

		// localizing the weapon name
		std::wstring localalized_name = n_interfaces::localize->find( name.c_str( ) );

		// we like lower case out here
		std::transform( localalized_name.begin( ), localalized_name.end( ), localalized_name.begin( ), std::tolower );

		// draw the weapons name
		g_menu.m_render.text( g_menu.m_render.m_fonts.tahoma, localalized_name, this->m_box.x + this->m_box.w / 2, this->m_box.y + this->m_box.h - 1, n_render::c_graphics::CENTERED_X, color_t( r, g, b, a ) );
	}

	void c_visuals::draw_health( n_sdk::C_CSPlayer* player, const int a ) {
		// creating a health variable on the stack
		const int health = std::clamp( player->get_health( ), 0, 100 ); // clamping because of aids gamemode i forgot the name of

		// creating a height variable to use when drawing
		const int height = health * this->m_box.h / 100;

		// creating three seperate ints for the colour of the health bar
		const int r = static_cast< int >( 255 - health * 2.55 ),
			g = static_cast< int >( health * 2.55 ),
			b = 0;

		// outline rectangles
		g_menu.m_render.filled_rect( this->m_box.x - 7, this->m_box.y - 1, 5, this->m_box.h + 2, color_t( 10, 10, 10, a ) );
		// main rectangle
		g_menu.m_render.filled_rect( this->m_box.x - 6, this->m_box.y + this->m_box.h - height, 3, height, color_t( r, g, b, a ) );
		// draw the health amount
		g_menu.m_render.text( g_menu.m_render.m_fonts.tahoma, fmt::format_int( health ).str( ), this->m_box.x - 8, this->m_box.y + this->m_box.h - height, n_render::c_graphics::RIGHT | n_render::c_graphics::CENTERED_Y, color_t( 255, 255, 255, a ) );
	}

	void c_visuals::paint( void ) {
		// draw info because gansta
		this->draw_watermark( );

		// draw sexc notifications
		this->draw_notifications( );

		// check if we are connected to a server
		if ( !n_interfaces::engine->is_connected( ) )
			return;

		// do player loop ( could later be changed to loop through all the entities if we want to do other visual stuff )
		for ( auto& players : n_globals::players ) {
			// creating a player variable on the stack for "tideness"
			n_sdk::C_CSPlayer* player = players.m_player;

			// sanity checks
			if ( !player || !player->is_alive( ) || player->networkable( )->is_dormant( ) || player->is_in_same_team( n_globals::local_player ) )
				continue;

			// checking if we can successfully create the intended box
			if ( !this->calculate_box( player ) )
				// we cant draw anything on the  player without the box
				continue;

			// draw the box
			this->draw_box( 255, 255, 255, 220 );

			// draw the name
			this->draw_name( player, 255, 255, 255, 220 );

			// draw the weapon name
			this->draw_weapon( player, 255, 255, 255, 220 );

			// draw the health
			this->draw_health( player, 220 );
		}
	}
}