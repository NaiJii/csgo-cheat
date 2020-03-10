#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		int __fastcall ListLeavesInBox( std::uintptr_t ecx, std::uintptr_t edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max ) {
			static auto original_fn = vmt_manager->get_original_fn< int( __thiscall* )( std::uintptr_t, const vec3_t&, const vec3_t&, unsigned short*, int ) >( ListLeavesInBox );

			// CClientLeafSystem::InsertIntoTree sig "55 8B EC 83 E4 F8 81 EC ? ? ? ? 8B 45 08 53 56 8B 75"
			// [...]
			// .text:1024D4D7                   call    dword ptr[ eax + 18h ]      // called here
			// .text:1024D4DA                   mov[ esp + 930h + leaf_count ], eax // return here ( so u should sig this one... )
			// .text:1024D4DE                   jmp     short loc_1024D4E8          // :)

			static auto list_leaves_in_box_return = n_utilities::find_pattern( "client_panorama.dll", "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" );

			// only disabling model occlussion when ListLeavesInBox is called
			if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) != list_leaves_in_box_return )
				return original_fn( ecx, mins, maxs, list, list_max );

			// RenderableInfo_t struct is located at _AddressOfReturnAddress + 0x14 - i am too cool for the struct :sunglasses:
			const auto renderable_info = *reinterpret_cast< void** >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 );

			// IClientRenderable pointer is located at renerable_info + 0x0
			n_sdk::IClientRenderable* renderable = *reinterpret_cast< n_sdk::IClientRenderable** >( renderable_info );

			// sanity checks
			if ( !renderable_info || !renderable )
				return original_fn( ecx, mins, maxs, list, list_max );

			// getting the base entity from the renderable pointer
			n_sdk::C_BaseEntity* entity = renderable->get_client_unknown( )->get_base_entity( );

			// sanity checks
			if ( !entity || !entity->is_player( ) )
				return original_fn( ecx, mins, maxs, list, list_max );

			// accessing other members of the RenderableInfo_t struct
			// fix render order by forcing translucent group
			*reinterpret_cast< std::uint16_t* >( reinterpret_cast< std::uintptr_t >( renderable_info ) + 0x16 ) &= ~0x100;
			*reinterpret_cast< std::uint16_t* >( reinterpret_cast< std::uintptr_t >( renderable_info ) + 0x18 ) |= 0xC0;

			// creating some variables at compile time to fix world space bounds
			const constexpr float MAX_COORD_FLOAT = 16384.f;
			const constexpr float MIN_COORD_FLOAT = -MAX_COORD_FLOAT;

			// creating two static variable to fix world space bounds
			static vec3_t map_min = vec3_t( MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT );
			static vec3_t map_max = vec3_t( MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT );

			// fixing world space bounds
			return original_fn( ecx, map_min, map_max, list, list_max );
		}
	}
}