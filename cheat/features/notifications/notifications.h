#pragma once

#include <string>

namespace n_features {
	class c_notification {
	public:
		c_notification( const std::string& notification );
		
		// class variables used to handle each notification
		float m_spawn_time;
		std::string m_message;

		/* config */
		struct {
			int x = 10, y = 10;
			float life_span = 5.f;
		} m_config;	
	};
}