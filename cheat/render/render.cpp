#include "render.h"

namespace n_render {
    bool n_render::initialize(IDirect3DDevice9* device) {
        if ( !device )
            return false;

        n_directx::device = device;
        return true;
    }

    /* Drawing functions */
    void draw_filled_rect( const int x, const int y, const int w, const int h, color_t color ) {
        static D3DRECT rect;
        rect = { x, y, x + w, y + h };
        n_directx::device->Clear( 1, &rect, D3DCLEAR_TARGET, D3DCOLOR_RGBA( color.r, color.g, color.b, color.a ), 0.f, 0 );
    }














	bool c_graphics::get( void ) {
		auto create_font = [ & ]( n_sdk::HFont& font, const std::string name, const int size, const int weight, const int flags ) -> bool {
			font = n_interfaces::surface->create_font( );

			if ( !font )
				return false;

			n_interfaces::surface->set_font_glyph_set( font, name.c_str( ), size, weight, 0, 0, flags, 0, 0 );

			return true;
		};

		if ( !create_font( this->m_fonts.tahoma, "Tahoma", 12, 400, n_sdk::FONTFLAG_DROPSHADOW ) )
			return false;

		n_interfaces::surface->get_screen_size( this->m_screen_size.x, this->m_screen_size.y );

		if ( !this->m_screen_size.x || !this->m_screen_size.y )
			return false;

		return true;
	}

	void c_graphics::filled_rect( const int x, const int y, const int w, const int h, const color_t& color ) {
		n_interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
		n_interfaces::surface->draw_filled_rect( x, y, x + w, y + h );
	}

	void c_graphics::outlined_rect( const int x, const int y, const int w, const int h, const color_t& color ) {
		n_interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
		n_interfaces::surface->draw_outlined_rect( x, y, x + w, y + h );
	}

	void c_graphics::line( const int x, const int y, const int w, const int h, const color_t& color ) {
		n_interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
		n_interfaces::surface->draw_line( x, y, x + w, y + h );
	}

	void c_graphics::get_text_size( const n_sdk::HFont& font, const std::string_view text, int& width, int& height ) {
		std::wstring converted_text = std::wstring( text.begin( ), text.end( ) );
		n_interfaces::surface->get_text_size( font, converted_text.c_str( ), width, height );
	}

	std::pair< int, int > c_graphics::text( const n_sdk::HFont& font, const std::string_view text, const int x, const int y, const int flags, const color_t& color ) {
		std::wstring converted_text = std::wstring( text.begin( ), text.end( ) );

		int width, height;
		n_interfaces::surface->get_text_size( font, converted_text.c_str( ), width, height );

		int _x = x, _y = y;

		if ( flags ) {
			if ( flags & CENTERED_X )
				_x -= width / 2;
			else if ( flags & RIGHT )
				_x -= width;

			if ( flags & CENTERED_Y )
				_y -= height / 2;
			else if ( flags & BOTTOM_Y )
				_y -= height;
		}

		n_interfaces::surface->draw_set_text_color( color.r, color.g, color.b, color.a );
		n_interfaces::surface->draw_set_text_font( font );

		n_interfaces::surface->draw_set_text_pos( _x, _y );

		n_interfaces::surface->draw_print_text( converted_text.c_str( ), converted_text.length( ) );
		
		return { width, height };
	}

	std::pair< int, int > c_graphics::text( const n_sdk::HFont& font, const std::wstring_view text, const int x, const int y, const int flags, const color_t& color ) {
		int width, height;
		n_interfaces::surface->get_text_size( font, text.data( ), width, height );

		int _x = x, _y = y;

		if ( flags ) {
			if ( flags & CENTERED_X )
				_x -= width / 2;
			else if ( flags & RIGHT )
				_x -= width;

			if ( flags & CENTERED_Y )
				_y -= height / 2;
			else if ( flags & BOTTOM_Y )
				_y -= height;
		}

		n_interfaces::surface->draw_set_text_color( color.r, color.g, color.b, color.a );
		n_interfaces::surface->draw_set_text_font( font );

		n_interfaces::surface->draw_set_text_pos( _x, _y );

		n_interfaces::surface->draw_print_text( text.data( ), text.length( ) );

		return { width, height };
	}
}