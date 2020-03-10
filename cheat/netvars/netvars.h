#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../interfaces/interfaces.h"

// https://gist.github.com/AltimorTASDK/75fe97ff3174a7de3739

namespace n_netvars {
	class c_netvar_tree {
		struct node_t;
		using map_type = std::unordered_map< std::string, std::shared_ptr< node_t > >;

		struct node_t {
			node_t( const int offset ) : offset( offset ) { }
			map_type nodes;
			int offset;
		};

		map_type nodes;

	public:
		bool setup( void );

	private:
		void populate_nodes( class n_sdk::RecvTable* recv_table, map_type* map );

		/**
		 * get_offset_recursive - Return the offset of the final node
		 * @map:	Node map to scan
		 * @acc:	Offset accumulator
		 * @name:	Netvar name to search for
		 *
		 * Get the offset of the last netvar from map and return the sum of it and accum
		 */
		int get_offset_recursive( map_type& map, int acc, const char* name ) {
			return acc + map[ name ]->offset;
		}

		/**
		 * get_offset_recursive - Recursively grab an offset from the tree
		 * @map:	Node map to scan
		 * @acc:	Offset accumulator
		 * @name:	Netvar name to search for
		 * @args:	Remaining netvar names
		 *
		 * Perform tail recursion with the nodes of the specified branch of the tree passed for map
		 * and the offset of that branch added to acc
		 */
		template< typename ...t >
		int get_offset_recursive( map_type& map, int acc, const char* name, t ...args ) {
			const auto& node = map[ name ];
			return get_offset_recursive( node->nodes, acc + node->offset, args... );
		}

	public:
		/**
		 * get_offset - Get the offset of a netvar given a list of branch names
		 * @name:	Top level datatable name
		 * @args:	Remaining netvar names
		 *
		 * Initiate a recursive search down the branch corresponding to the specified datable name
		 */
		template< typename ...t >
		int get_offset( const char* name, t ...args ) {
			const auto& node = nodes[ name ];
			return get_offset_recursive( node->nodes, node->offset, args... );
		}
	};

	// create a global pointer to access the class
	extern std::unique_ptr< c_netvar_tree > netvar_tree;

	bool setup( void );
}

#define netvar_fn( function_name, type, ... ) \
	type& function_name { \
		return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + n_netvars::netvar_tree->get_offset( __VA_ARGS__ ) ); \
	};

#define offset_fn( function_name, type, offset ) \
	type& function_name { \
		return *reinterpret_cast< type* >( std::uintptr_t( this ) + offset ); \
	};