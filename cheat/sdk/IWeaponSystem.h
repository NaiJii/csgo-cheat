#pragma once

#include "../utilities/utilities.h"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/231080-getting-wpndata-vtable-indexes.html

namespace n_sdk {
	struct CCSWeaponData {
		char _pad0[ 0x4 ];
		char* m_console_name;
		char _pad1[ 0xC ];
		int m_max_clip1;
		int m_max_clip2;
		int m_default_clip1;
		int m_default_clip2;
		int m_primary_reserve_ammo_max;
		int m_secondary_reserve_ammo_max;
		char* m_world_model;
		char* m_view_model;
		char* m_dropped_model;
		char _pad2[ 0x50 ];
		char* m_hud_name;
		char* m_weapon_name;
		char _pad3[ 0x2 ];
		bool m_is_melee_weapon;
		char _pad4[ 0x9 ];
		float m_weapon_weight;
		char _pad5[ 0x28 ];
		int m_weapon_type;
		char _pad6[ 0x4 ];
		int m_weapon_price;
		int m_kill_award;
		char _pad7[ 0x4 ];
		float m_cycle_time;
		float m_cycle_time_alt;
		char _pad8[ 0x8 ];
		bool m_full_auto;
		char _pad9[ 0x3 ];
		int m_damage;
		float m_armor_ratio;
		int m_bullets;
		float m_penetration;
		char _pad10[ 0x8 ];
		float m_weapon_range;
		float m_range_modifier;
		float m_throw_velocity;
		char _pad11[ 0xC ];
		bool m_has_silencer;
		char _pad12[ 0xB ];
		char* m_bullet_type;
		float m_max_speed;
		char _pad13[ 0x50 ];
		int m_recoil_seed;
	};

	class IWeaponSystem {
		virtual void unused0( ) = 0;
		virtual void unused1( ) = 0;
	public:
		virtual CCSWeaponData* get_weapon_data( unsigned index ) const = 0;
	};
}