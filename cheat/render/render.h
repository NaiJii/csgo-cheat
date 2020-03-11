#pragma once

#include "../interfaces/interfaces.h"
#include "../math/color.h"
#include "directx.h"

namespace n_render {
    bool initialize( IDirect3DDevice9* device );

    /* Drawing functions */
    void draw_filled_rect( const int x, const int y, const int w, const int h, color_t color );












	class c_graphics {
	public:
		enum {
			CENTERED_Y = 1,
			CENTERED_X = ( 1 << 1 ),
			BOTTOM_Y = ( 1 << 2 ),
			RIGHT = ( 1 << 3 ),
		};

		struct {
			n_sdk::HFont tahoma;
		} m_fonts;

		struct {
			int x, y;
		} m_screen_size;

		// initialiser
		bool get( void );

		// wrapper functions
		void filled_rect( const int x, const int y, const int w, const int h, const color_t& color );
		void outlined_rect( const int x, const int y, const int w, const int h, const color_t& color );
		void line( const int x, const int y, const int w, const int h, const color_t& color );
		void get_text_size( const n_sdk::HFont& font, const std::string_view text, int& width, int& height );
		std::pair< int, int > text( const n_sdk::HFont& font, const std::string_view text, const int x, const int y, const int flags, const color_t& color );
		std::pair< int, int > text( const n_sdk::HFont& font, const std::wstring_view text, const int x, const int y, const int flags, const color_t& color );

		/* w2c function */
		inline bool world_to_screen( const vec3_t& origin, vec3_t& screen ) {
			return n_interfaces::debug_overlay->screen_position( origin, screen ) != 1;
		}
	};
}