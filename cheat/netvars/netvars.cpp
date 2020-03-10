#include "netvars.h"

namespace n_netvars {
	/**
	 * netvar_tree - Constructor
	 *
	 * Call populate_nodes on every RecvTable under client->GetAllClasses()
	 */
	bool c_netvar_tree::setup( void ) {
		const n_sdk::ClientClass* client_class = n_interfaces::client_dll->get_all_classes( );

		if ( !client_class )
			return false;

		while ( client_class != nullptr ) {
			const auto class_info = std::make_shared< node_t >( 0 );

			n_sdk::RecvTable* recv_table = client_class->m_table;

			populate_nodes( recv_table, &class_info->nodes );

			nodes.emplace( recv_table->m_net_table_name, class_info );

			client_class = client_class->m_next;
		}

		return true;
	}

	/**
	 * populate_nodes - Populate a node map with brances
	 * @recv_table:	Table the map corresponds to
	 * @map:	Map pointer
	 *
	 * Add info for every prop in the recv table to the node map. If a prop is a
	 * datatable itself, initiate a recursive call to create more branches.
	 */
	void c_netvar_tree::populate_nodes( n_sdk::RecvTable* recv_table, map_type* map ) {
		for ( auto i = 0; i < recv_table->m_props; i++ ) {
			const n_sdk::RecvProp* prop = &recv_table->m_prop[ i ];

			const auto prop_info = std::make_shared< node_t >( prop->m_offset );

			if ( prop->m_recv_type== n_sdk::DPT_DataTable )
				populate_nodes( prop->m_data_table, &prop_info->nodes );

			map->emplace( prop->m_var_name, prop_info );
		}
	}

	// resolving the extern declaration
	std::unique_ptr< c_netvar_tree > netvar_tree;

	bool setup( void ) {
		netvar_tree = std::make_unique< c_netvar_tree >( );

		// if we successfully initialized the netvar class
		if ( !netvar_tree->setup( ) )
			return false;

		// everything went good
		return true;
	}
}