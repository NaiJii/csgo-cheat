#include "../hooks.h"

#include "../../features/notifications/notifications.h"
#include "../../fmt/format.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall FireEvent( std::uintptr_t ecx, std::uintptr_t edx, n_sdk::IGameEvent* event, bool done_broadcast ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t, n_sdk::IGameEvent*, bool ) >( FireEvent );

			// player_hurt event
			if ( std::strcmp( event->get_name( ), "player_hurt" ) == 0 ) {
				// creating some variables on the stack about the event
				const int target = n_interfaces::engine->get_player_for_user_id( event->get_int( "userid" ) );
				const int attacker = n_interfaces::engine->get_player_for_user_id( event->get_int( "attacker" ) );

				// variable on the stack for the local player id
				const int local_player = n_interfaces::engine->get_local_player( );

				// sanity check
				if ( target != local_player && attacker == local_player ) {
					// creating a player_info_t varaible on the stack
					n_sdk::player_info_t info{ };

					// getting the player info
					if ( !n_interfaces::engine->get_player_info( target, &info ) )
						return original_fn( ecx, event, done_broadcast );

					// yea i guess this can just go here for now
					auto hitgroup_to_name = [ ]( const int hitgroup ) -> std::string {
						switch ( hitgroup ) {
						case 1:
							return "head";
						case 3:
							return "stomach";
						case 6:
						case 7:
							return "leg";
						default:
							return "body";
						}
					};

					// creating a string variable on the stack for the message
					std::string message = "hit " + std::string( info.m_name ) + " in " + hitgroup_to_name( event->get_int( "hitgroup" ) ) +  " for " + fmt::format_int( event->get_int( "dmg_health" ) ).str( );

					// we like lower case out here
					std::transform( message.begin( ), message.end( ), message.begin( ), std::tolower );

					// add the notification to the deque
					n_globals::notifications.push_back( n_features::c_notification( message ) );
				}
			}

			return original_fn( ecx, event, done_broadcast );
		}
	}
}