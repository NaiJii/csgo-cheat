#pragma once

#include "../../globals/globals.h"

namespace n_features {
	class c_visuals {
	private:
		struct {
			int x, y, w, h;
		} m_box;

		struct {
			int x, y;
			std::pair< int, int > size;
		} m_info;

		/* misc functions */
		bool calculate_box( n_sdk::C_BaseEntity* entity );
		void draw_watermark( void );
		void draw_notifications( void );
		void draw_server_hitboxes( void );

		/* player functions */
		void draw_box( const int r, const int g, const int b, const int a );
		void draw_name( n_sdk::C_CSPlayer* player, const int r, const int g, const int b, const int a );
		void draw_weapon( n_sdk::C_CSPlayer* player, const int r, const int g, const int b, const int a );
		void draw_health( n_sdk::C_CSPlayer* player, const int a );

	public:
		/* called to draw stuff */
		void paint( void );
	};
}

extern n_features::c_visuals g_visuals;