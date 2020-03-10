#pragma once
#include <string>

namespace n_menu {
    class c_slider {
    public:
        c_slider( std::string_view name, std::string_view tooltip, const int min, const int max, int* value ) {
            m_text = name;
            m_tooltip = tooltip;
            if ( min < max ) {
                m_min = min;
                m_max = max;
            }
            else if ( min > max ) {
                m_max = min;
                m_min = max;
            }
            m_value = value;
        }
        std::string m_text;
        std::string m_tooltip;
        int m_min;
        int m_max;
        int* m_value;
    };
}