#pragma once

#include "../interfaces/interfaces.h"
#include "../render/render.h"
#include "../math/color.h"
#include "../fmt/format.h"

#include "objects/tab.h"
#include "objects/checkbox.h"
#include "objects/sliders.h"

#include <functional>
#include <array>

namespace n_menu {

	constexpr auto g_tab_allowance = 30;
    constexpr auto g_control_spacing_x = 20;
    constexpr auto g_control_spacing_y = 20;

	static auto is_in_bounds = [ ]( int x, int y, int x2, int y2, int mx, int my ) {

		return mx > x&& my > y&& mx < x2 && my < y2;

	};

	class c_menu {
	private:

		struct {
			int x, y;
			int width, height;
			bool open;
			int selected_tab;

            std::array<bool, 256> m_key_down;
            std::array<bool, 256> m_prev_key_down;
			
			struct {

				int offset_x, offset_y;
				bool is_dragging;
				bool hovering_draggable;

			} m_dragging;

			bool is_hovering_backplate;

		} m_config;

        color_t m_theme;

		int m_mouse_x, m_mouse_y;
        int m_cursor_x, m_cursor_y;

		std::vector< c_tab > m_tabs;
        std::vector< c_checkbox > m_checkboxes;
        std::vector< c_slider > m_sliders;

		std::string m_display_tooltip;

		void set_tooltip( std::string );

	public:

		// renderer
		n_render::c_graphics m_render;

		// dont be fooled! initialisers
		bool get( void );
		void make_tab( std::string, std::string, std::function< void( c_tab& ) > );
        void make_checkbox( std::string_view, std::string_view, bool* );
        void make_slider( std::string_view, std::string_view, const int, const int, int* );

		/* Input Function */
		bool input( int, int );

		/* Rendering Functions */
		void paint( void );
		void draw_backplate( void );
        void draw_tooltip( void );

		void draw_tabs( void );
        void draw_checkboxes( void );
        void draw_sliders( void );
		
		/* Acess Private Struct Members */
		inline bool is_opened( void ) {
			return this->m_config.open;
		}

        inline bool should_remove_input( void ) {
            return !this->m_config.is_hovering_backplate && this->m_config.open;
        }

        inline bool is_key_down( unsigned int key ) {
            return this->m_config.m_key_down[key];
        }

        inline bool is_key_pressed( unsigned int key ) {
            return !this->m_config.m_key_down[key] && this->m_config.m_prev_key_down[key];
        }

        inline void toggle( unsigned int key, bool& var ) {
            if ( is_key_pressed( key ) ) var = !var;
        }
	};
}

extern n_menu::c_menu g_menu;