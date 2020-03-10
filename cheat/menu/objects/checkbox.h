#pragma once
#include <string>

namespace n_menu {
    class c_checkbox {
    public:
        c_checkbox( std::string_view name, std::string_view tooltip, bool* value ) {
            m_text = name;
            m_tooltip = tooltip;
            m_value = value;
        }
        std::string m_text;
        std::string m_tooltip;
        bool* m_value;
    };
}