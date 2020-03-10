#include "vmt.h"

#include <memoryapi.h>
#include <cstring>

#include "../utilities/utilities.h"

// for when i lose my marbles - https://www.unknowncheats.me/forum/2142031-post5.html

namespace n_vmt {
	bool c_vmt::setup( void* base_class ) {
		// getting the class pointer from the base class
		// cast the base class pointer to a two level pointer because the pointer behind the class pointer is the vft pointer
		auto class_ptr = reinterpret_cast< std::uintptr_t** >( base_class );

		// create a variable on the stack to store the vft pointer
		auto vft_ptr = *class_ptr;

		// count the number of virtual functions in the vft ( limit the loop to 1000 incase of iterating through entire programs memory )
		while ( vft_ptr[ this->m_function_count ] && this->m_function_count <= 1000 )
			++this->m_function_count;

		// if there are no virtual functions then there is nothing to hook so return
		if ( this->m_function_count == 0 )
			return false;

		// allocate memory so we can copy the vtf
		this->m_vft.m_original = reinterpret_cast< std::uintptr_t* >( VirtualAlloc( nullptr, this->m_function_count * sizeof( void* ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ) );

		// if allocating memory to the "original" failed
		if ( !this->m_vft.m_original )
			return false;

		// copy the memory to the "original" class member
		std::memcpy( this->m_vft.m_original, vft_ptr, this->m_function_count * sizeof( void* ) );

		// store original vft 
		this->m_vft.m_normal = vft_ptr;

		// successfully setup the class
		return true;
	}

	bool c_vmt::hook( const std::uintptr_t index, void* function ) {
		// if the index is invalid then we cannot hook the function
		if ( index < 0 || index >= this->m_function_count )
			return false;

		// protect memory region and store old page protection
		const auto old_protection = n_utilities::protect_full_memory_region( this->m_vft.m_normal, PAGE_EXECUTE_READWRITE );

		// override existing function pointer
		this->m_vft.m_normal[ index ] = reinterpret_cast< std::uintptr_t >( function );

		// revert page protection
		n_utilities::protect_full_memory_region( this->m_vft.m_normal, old_protection );

		// successfully replaces virtual function pointer
		return true;
	}

	bool c_vmt::unload( void ) {
		// if the vft hasnt been copied to the "original" or "normal"
		if ( !this->m_vft.m_original || !this->m_vft.m_normal )
			return false;

		// make pages able to be read and wrote to
		const auto old_protection = n_utilities::protect_full_memory_region( this->m_vft.m_normal, PAGE_EXECUTE_READWRITE );

		// copy original vft ptrs to vmt
		std::memcpy( this->m_vft.m_normal, this->m_vft.m_original, this->m_function_count * sizeof std::uintptr_t );

		// revert page protection
		n_utilities::protect_full_memory_region( this->m_vft.m_normal, old_protection );

		// prevents memory leaks ( frees allocated memory )
		VirtualFree( this->m_vft.m_original, 0, MEM_RELEASE );

		// successfully retores the vtf
		return true;
	}

	bool c_vmt_manager::hook_vft( void* base_class, std::function< bool( c_vmt* ) > function ) {
		// temporarily create in instance of c_vmt on the stack
		c_vmt vft;

		// attempt to setup the vft
		if ( !vft.setup( base_class ) )
			return false;

		// the function will consist of a lamda which attempts to hook functions in the vft
		// if replacing the virtual function pointer fails then we cannot continue
		if ( !function( &vft ) ) {
			// attempt to restore the vft
			vft.unload( );
			// the vft has not been successfully hooked
			return false;
		}

		// if we have successfully hooked the vft and the desired virtual functions we can push the vft onto the vector
		this->m_vfts.push_back( std::move( vft ) );

		// successfully hooked the vtf
		return true;
	}

	bool c_vmt_manager::unload( void ) {
		// iterate over hooked vfts
		for ( auto vft : this->m_vfts )
			// if we did not successfully free memory and revert vft
			if ( !vft.unload( ) )
				// return false as there has already been a memory leak and an issue needs to be fixed ;)
				return false;

		// if we successfully reverted all of the vfts and freed the allocated memory
		// we can clear the vector
		this->m_vfts.clear( );

		// successfully unhooked the vtf
		return true;
	}
}