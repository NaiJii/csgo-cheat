#pragma once

#include <windows.h>
#include <cstdint>
#include <vector>
#include <functional>

namespace n_vmt {
	class c_vmt {
		friend class c_vmt_manager; // lol this is probably a bad idea but i dont want all the class members being public
	public:
		// used for copying the virtual function table ( vft )
		bool setup( void* base_class );

		// used for replacing an existing function pointer with or own
		bool hook( const std::uintptr_t index, void* function );

		// used to revert all virtual functions back to there originals - restores the original vft
		bool unload( void );
		
	private:
		// used for storing the number of virtual functions in the vft
		std::uintptr_t m_function_count{ 0 };

		// anonymouse struct to store information about the vft
		struct {
			// used for storing the all of the virtual functions in the vft ( doesnt get modified )
			std::uintptr_t* m_original{ nullptr };

			// used for overriding the address's of the virtual functions in the vft to our own function pointer ( gets modified )
			std::uintptr_t* m_normal{ nullptr };
		} m_vft;

	};

	class c_vmt_manager {
	public:
		// hooks vft and uses lamda in conjunction with std::function to hook virtual functions
		bool hook_vft( void* base_class, std::function< bool( c_vmt* ) > function );

		// restores all of the virtual functions in the vft
		bool unload( void );

		// return the original function of the virtual function
		template< typename t > t get_original_fn( void* function ) {
			// iterate over hooked vfts
			for ( auto vft : this->m_vfts )
				// iterate through all the virtual functions in the vtf
				for ( std::uintptr_t i = 0; i < vft.m_function_count; i++ )
					// if the hooked function matches 
					if ( vft.m_vft.m_normal[ i ] == reinterpret_cast< std::uintptr_t >( function ) )
						// return the original
						return ( t )vft.m_vft.m_original[ i ];

			// if we didnt find any match
			return nullptr;
		}

		// unhook SPECIFIC vtable 
		template< typename t > bool unhook( t base_class ) {
			// getting the class pointer from the base class
			// cast the base class pointer to a two level pointer because the pointer behind the class pointer is the vft pointer
			auto class_ptr = reinterpret_cast< std::uintptr_t** >( base_class );

			// create a variable on the stack to store the vft pointer
			auto vft_ptr = *class_ptr;

			const auto iterator = std::find_if( this->m_vfts.begin( ), this->m_vfts.end( ), [ & ]( const c_vmt& vmt ) {
				return vmt.m_vft.m_normal == vft_ptr;
				} );

			// if the iterator is valid
			if ( this->m_vfts.end( ) != iterator ) {
				// unload the vtable
				this->m_vfts.at( std::uintptr_t( iterator - this->m_vfts.begin( ) ) ).unload( );

				// remove it from the list
				this->m_vfts.erase( iterator );

				// function succeeded
				return true;
			}
			return false;
		}

	private:
		// used for storing all of the vfts hooked
		std::vector< c_vmt > m_vfts{ };
	};
}