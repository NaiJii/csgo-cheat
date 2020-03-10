#pragma once

#include "../math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/glow_outline_effect.h

namespace n_sdk {
	class C_BaseEntity;

	enum glow_style_t : int {
		GLOW_STYLE_NORMAL = 0,
		GLOW_STYLE_MODEL,
		GLOW_STYLE_INLINE,
		GLOW_STYLE_INLINE_PULSATE,
		GLOW_STYLE_MAX
	};

	class CGlowObjectManager {
	public:
		static const int GLOW_FOR_ALL_SPLIT_SCREEN_SLOTS = -1;

		class GlowObjectDefinition_t { // should be a struct
		public:
			bool should_draw( int slot ) const {
				return this->get_entity( ) &&
					( this->m_split_screen_slot == GLOW_FOR_ALL_SPLIT_SCREEN_SLOTS || this->m_split_screen_slot == slot ) &&
					( this->m_render_when_occluded || this->m_render_when_unoccluded );
			}

			C_BaseEntity* get_entity( ) const {
				return this->m_entity;
			}

			bool is_unused( ) const {
				return this->m_next_free_slot != this->ENTRY_IN_USE;
			}

			void set( const float r, const float g, const float b, const float a, const int style ) {
				this->m_glow_color = vec3_t( r, g, b );
				this->m_glow_alpha = a;
				this->m_render_when_occluded = true;
				this->m_render_when_unoccluded = false;
				this->m_bloom_amount = 1.0f;
				this->m_style = style;
			}

			C_BaseEntity* m_entity;
			vec3_t m_glow_color;
			float m_glow_alpha;
			char _pad0[ 0x8 ];
			float m_bloom_amount;
			char _pad1[ 0x4 ];
			bool m_render_when_occluded;
			bool m_render_when_unoccluded;
			char _pad2[ 0x6 ];
			int m_style;
			int m_split_screen_slot;
			// linked list of free slots
			int m_next_free_slot;

			// special values for GlowObjectDefinition_t::m_nNextFreeSlot
			static const int END_OF_FREE_LIST = -1;
			static const int ENTRY_IN_USE = -2;
		};

		GlowObjectDefinition_t* m_glow_object_definitions; // should be a CUtlVector and a struct
		char _pad0[ 0x8 ];
		int m_size;
	};
}