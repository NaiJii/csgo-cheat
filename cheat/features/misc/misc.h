#pragma once

#include "../../globals/globals.h"

namespace n_features {
	class c_misc {
	private:
		void bunny_hop( void );
		void fake_lag( void );

		int m_move_type;
		int m_flags;
	public:
		void run( void );
	};
}

extern n_features::c_misc g_misc;