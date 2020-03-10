#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		void __fastcall DrawModel( std::uintptr_t ecx, std::uintptr_t edx, void* results, void* info, void* bone_to_world, float* flex_weights, float* flex_delayed_weights, vec3_t& model_origin, int flags ) {
			static auto original_fn = vmt_manager->get_original_fn< void( __thiscall* )( std::uintptr_t, void*, void*, void*, float*, float*, vec3_t&, int ) >( DrawModel );

			// IClientRenderable pointer is located at info + 0x18
			n_sdk::IClientRenderable* renderable = *reinterpret_cast< n_sdk::IClientRenderable** >( reinterpret_cast< std::uintptr_t >( info ) + 0x18 );

			// sanity check
			if ( !renderable )
				return original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

			// entity is always 0x4 behind IClientRenderable pointer so we can just
			n_sdk::C_BaseEntity* entity = reinterpret_cast< n_sdk::C_BaseEntity* >( reinterpret_cast< std::uintptr_t >( renderable ) - 0x4 );

			// sanity check
			if ( !entity )
				return original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

			// sanity checks
			if ( entity->is_player( ) && entity->is_alive( ) && !entity->is_in_same_team( n_globals::local_player ) ) {
				// fix for glow https://www.unknowncheats.me/forum/counterstrike-global-offensive/319515-glow-drawmodel-chams.html
				if ( n_interfaces::model_render->is_forced_material_override( ) )
					return original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

				// find the material
				static n_sdk::IMaterial* material = n_interfaces::material_system->get_material( "debug/debugambientcube", "Other textures" );

				// sanity check
				if ( !material )
					return original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

				// create a variable on the stack to store the color of the material
				const float color_invis[ ] = { 255.f / 255.f, 192.f / 255.f, 203.f / 255.f };

				// modulate the color of the material
				n_interfaces::studio_render->set_color_modulation( color_invis );

				// alpha modulate the material
				n_interfaces::studio_render->set_alpha_modulation( 1.f );

				// set material flags
				material->set_material_var_flag( n_sdk::MATERIAL_VAR_IGNOREZ, true );

				// override games material so it uses our material
				n_interfaces::studio_render->forced_material_override( material );

				// call the original virtual function
				original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

				// create another variable on the stack to store the color of the material
				const float color_vis[ ] = { 173.f / 255.f, 255.f / 255.f, 47.f / 255.f };

				// modulate the color of the material
				n_interfaces::studio_render->set_color_modulation( color_vis );

				// alpha modulate the material
				n_interfaces::studio_render->set_alpha_modulation( 1.f );

				// set material flags
				material->set_material_var_flag( n_sdk::MATERIAL_VAR_IGNOREZ, false );

				// override games material so it uses our material
				n_interfaces::studio_render->forced_material_override( material );

				// call the original virtual function
				original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );

				// revert override material
				n_interfaces::studio_render->forced_material_override( nullptr );

				// return so we dont draw a model over our material
				return;
			}

			// return the original virtual function
			return original_fn( ecx, results, info, bone_to_world, flex_weights, flex_delayed_weights, model_origin, flags );
		}
	}
}