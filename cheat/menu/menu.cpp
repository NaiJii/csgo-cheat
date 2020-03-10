#include "menu.h"

n_menu::c_menu g_menu{ };

namespace n_menu {

	// this is where the elements are initialised
	bool c_menu::get( void ) {

		if ( !this->m_render.get( ) )
			return false;

        static bool dummy_b = false;
        static int dummy_s = 20;

		this->make_tab( "aimbot", "rage aim configuration.", [ ]( c_tab& tab ) {} );
        this->make_checkbox( "checkbox", "this is a checkbox", &dummy_b );
        this->make_slider( "slider", "this is a slider", 10, 145, &dummy_s );

		this->make_tab( "visuals", "visual assistance configuration.", [ ]( c_tab& tab ) {} );

		this->make_tab( "other", "miscellaneous configuration.", [ ]( c_tab& tab ) {} );

		this->m_display_tooltip = "cheat made by Ji#1403";
		this->m_config.x = 10;
		this->m_config.y = 10;
		this->m_config.width = 700;
		this->m_config.height = 800;
		this->m_config.open = true;
		this->m_config.selected_tab = this->m_tabs.size( ) / 2;

        this->m_theme = color_t( 3, 252, 157 );

		return true;
	}

	void c_menu::set_tooltip( std::string tip ) {
		this->m_display_tooltip = tip.size( ) ? "tip: " + tip : "";
	}

	void c_menu::make_tab( std::string name, std::string tooltip, std::function<void( c_tab& )> fn ) {

		auto tab = c_tab( );

		tab.m_text = name;
		tab.m_tooltip = tooltip;

		fn( tab );

		this->m_tabs.push_back( std::move( tab ) );

	}

    void c_menu::make_checkbox( std::string_view name, std::string_view tooltip, bool* value ) {
        auto checkbox = c_checkbox( name, tooltip, value );
        this->m_checkboxes.push_back( std::move( checkbox ) );
    }

    void c_menu::make_slider( std::string_view name, std::string_view tooltip, const int min, const int max, int* value ) {
        auto slider = c_slider( name, tooltip, min, max, value );
        this->m_sliders.push_back( std::move( slider ) );
    }

	// paint
	void c_menu::paint( void ) {
        for ( int i = 0; i < 256; i++ ) {
            this->m_config.m_prev_key_down[i] = this->m_config.m_key_down[i];
            this->m_config.m_key_down[i] = GetAsyncKeyState( i );
        }

        this->toggle( VK_INSERT, this->m_config.open );

		if ( !this->m_config.open )
			return;

        if ( !is_key_down( VK_LBUTTON ) )
            this->m_config.m_dragging.is_dragging = false;

        if ( is_key_down( VK_LBUTTON ) && this->m_config.m_dragging.hovering_draggable ) {
            this->m_config.m_dragging.is_dragging = true;
            this->m_config.m_dragging.offset_x = this->m_config.x - this->m_mouse_x;
            this->m_config.m_dragging.offset_y = this->m_config.y - this->m_mouse_y;
        }

		this->draw_backplate( );
		this->draw_tabs( );
        this->draw_checkboxes( );
        this->draw_sliders( );
		this->draw_tooltip( );

	}

	void c_menu::draw_tabs( void ) {

		const auto space_for_tab = this->m_config.width / this->m_tabs.size( );
        const bool is_hovering_tabs = is_in_bounds( this->m_config.x, this->m_config.y, this->m_config.x + this->m_config.width, this->m_config.y + g_tab_allowance, this->m_mouse_x, this->m_mouse_y );

		for ( unsigned int i = 0; i < this->m_tabs.size( ); i++ ) {

			auto& tab = this->m_tabs[ i ];

			const bool is_selected = i == this->m_config.selected_tab;
            const bool is_hovered = i == ( this->m_mouse_x - this->m_config.x ) / ( this->m_config.width / this->m_tabs.size() ) && is_hovering_tabs;

            color_t color = color_t( 255, 255, 255, 70 );

            if ( is_hovered ) {
                color = m_theme;
                this->set_tooltip( tab.m_tooltip );
                if ( is_key_pressed( VK_LBUTTON ) )
                    m_config.selected_tab = i;
            }

            if ( is_selected )
                color.a = 255;

            auto x = this->m_config.x + space_for_tab * i + space_for_tab / 2;

            const auto size = this->m_render.text( this->m_render.m_fonts.tahoma, tab.m_text, x, this->m_config.y + g_tab_allowance / 2, n_render::c_graphics::CENTERED_Y, color );
            this->m_render.line( x, this->m_config.y + g_tab_allowance, size.first, 0, color );
		}

        this->m_cursor_x = this->m_config.x + g_control_spacing_x;
        this->m_cursor_y = this->m_config.y + g_tab_allowance + g_control_spacing_y;
	}

    void c_menu::draw_checkboxes( void ) {

        constexpr auto checkbox_size = 10;

        for ( unsigned int i = 0; i < this->m_checkboxes.size(); i++ ) {

            auto& checkbox = this->m_checkboxes[i];

            const bool is_selected = *checkbox.m_value;

            color_t color = color_t( 255, 255, 255, 70 );

            int text_width, text_height;
            this->m_render.get_text_size( this->m_render.m_fonts.tahoma, checkbox.m_text, text_width, text_height );

            const bool is_hovered = is_in_bounds( this->m_cursor_x, this->m_cursor_y, this->m_cursor_x + checkbox_size + text_width, this->m_cursor_y + checkbox_size, this->m_mouse_x, this->m_mouse_y );

            if ( is_hovered ) {
                color = m_theme;
                this->set_tooltip( checkbox.m_tooltip );
                this->toggle( VK_LBUTTON, *checkbox.m_value );
            }

            if ( is_selected )
                color.a = 255;

            this->m_render.text( this->m_render.m_fonts.tahoma, checkbox.m_text, this->m_cursor_x + checkbox_size + 4, this->m_cursor_y + checkbox_size / 2, n_render::c_graphics::CENTERED_Y, color );
            this->m_render.filled_rect( this->m_cursor_x, this->m_cursor_y, checkbox_size, checkbox_size, color );

            this->m_cursor_y += checkbox_size + g_control_spacing_y;
        }
    }
    
    void c_menu::draw_sliders( void ) {
        constexpr auto slider_width = 100;
        constexpr auto slider_height = 10;

        for ( unsigned int i = 0; i < this->m_sliders.size(); i++ ) {

            auto& slider = this->m_sliders[i];

            color_t color = color_t( 255, 255, 255, 70 );

            int text_width, text_height;
            this->m_render.get_text_size( this->m_render.m_fonts.tahoma, slider.m_text, text_width, text_height );

            static bool is_selected = false; // so even after grabbing the slider out of his size, the value changes.
            const bool is_hovered = is_selected || is_in_bounds( this->m_cursor_x, this->m_cursor_y, this->m_cursor_x + slider_width + text_width, this->m_cursor_y + slider_height, this->m_mouse_x, this->m_mouse_y );


            if ( !is_selected && is_hovered && this->is_key_down( VK_LBUTTON ) )
                is_selected = true;
            else if ( is_selected && !this->is_key_down( VK_LBUTTON ) )
                is_selected = false;
            else if ( is_selected && this->is_key_down( VK_LBUTTON ) ) {
                const float value_unmapped = std::clamp( static_cast<float>( this->m_mouse_x - this->m_cursor_x ), 0.f, static_cast<float>( slider_width ) );
                const int value_mapped = static_cast<int>( value_unmapped / slider_width * ( slider.m_max - slider.m_min ) + slider.m_min );
                *slider.m_value = value_mapped;
            }

            std::string indicator = slider.m_text;

            if ( is_hovered ) {
                color = m_theme;
                this->set_tooltip( slider.m_tooltip );
                indicator = fmt::format_int( *slider.m_value ).str();
            }

            if ( is_selected )
                color.a = 255;

            this->m_render.text( this->m_render.m_fonts.tahoma, indicator, this->m_cursor_x + slider_width + 4, this->m_cursor_y + slider_height / 2, n_render::c_graphics::CENTERED_Y, color );
            this->m_render.filled_rect( this->m_cursor_x, this->m_cursor_y, slider_width, slider_height, color_t( 45, 45, 45 ) ); //bg
            this->m_render.outlined_rect( this->m_cursor_x, this->m_cursor_y, slider_width, slider_height, color );
            this->m_render.filled_rect( this->m_cursor_x + 1, this->m_cursor_y + 1, static_cast<int>( static_cast<float>( *slider.m_value - slider.m_min ) / ( slider.m_max - slider.m_min ) * ( slider_width - 2 ) ), slider_height - 2, color_t( 0, 70, 200 ) );

            this->m_cursor_y += slider_height + g_control_spacing_y;
        }
    }

	void c_menu::draw_tooltip( void ) {

		if ( !this->m_display_tooltip.size( ) )
			return;

		int width, height;
		this->m_render.get_text_size( this->m_render.m_fonts.tahoma, this->m_display_tooltip.c_str( ), width, height );

		this->m_render.text( this->m_render.m_fonts.tahoma, this->m_display_tooltip, this->m_config.x + 2, this->m_config.y + this->m_config.height - height - 2, false, color_t( 255, 255, 255, 160 ) );

	}

	bool c_menu::input( int x, int y ) {
        this->m_mouse_x = x;
        this->m_mouse_y = y;

        this->m_config.is_hovering_backplate = this->m_config.open && is_in_bounds( this->m_config.x - 7, this->m_config.y - 7, this->m_config.x + this->m_config.width + 7, this->m_config.y + this->m_config.height + 7, x, y );

        if ( !this->m_config.open )
            return false;

        this->m_config.m_dragging.hovering_draggable = is_in_bounds( this->m_config.x - 7, this->m_config.y - 7, this->m_config.x, this->m_config.y + this->m_config.height + 7, x, y )
            || is_in_bounds( this->m_config.x - 7, this->m_config.y - 7, this->m_config.x + this->m_config.width + 7, this->m_config.y, x, y )
            || is_in_bounds( this->m_config.x - 7, this->m_config.y + this->m_config.height, this->m_config.x + this->m_config.width + 7, this->m_config.y + this->m_config.height + 7, x, y )
            || is_in_bounds( this->m_config.x + this->m_config.width, this->m_config.y - 7, this->m_config.x + this->m_config.width + 7, this->m_config.y + this->m_config.height + 7, x, y );

        if ( this->m_config.m_dragging.is_dragging ) {

            this->m_config.x = this->m_config.m_dragging.offset_x + x;
            this->m_config.y = this->m_config.m_dragging.offset_y + y;
            return true;
        }

        if ( !this->m_config.is_hovering_backplate )
            return false;

        this->m_display_tooltip = "";
        return true;

	}

	// background of menu
	void c_menu::draw_backplate( void ) {

		auto& r = this->m_render;

		const constexpr int size_of_grey_area = 4;

		auto buffer = size_of_grey_area + 3;

		r.outlined_rect( this->m_config.x - buffer, this->m_config.y - buffer, this->m_config.width + buffer + buffer, this->m_config.height + buffer + buffer, color_t( 30, 30, 30, 255 ) );

		buffer--;

		r.outlined_rect( this->m_config.x - buffer, this->m_config.y - buffer, this->m_config.width + buffer + buffer, this->m_config.height + buffer + buffer, color_t( 110, 110, 110, 255 ) );

		for ( int i = 0; i < size_of_grey_area; i++ ) {

			buffer--;

			r.outlined_rect( this->m_config.x - buffer, this->m_config.y - buffer, this->m_config.width + buffer + buffer, this->m_config.height + buffer + buffer, color_t( 70, 70, 70, 255 ) );

		}

		buffer--;

		r.outlined_rect( this->m_config.x - buffer, this->m_config.y - buffer, this->m_config.width + buffer + buffer, this->m_config.height + buffer + buffer, color_t( 110, 110, 110, 255 ) );

		r.filled_rect( this->m_config.x, this->m_config.y, this->m_config.width, this->m_config.height, color_t( 30, 30, 30, 255 ) );

	}
}