#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h

namespace n_sdk {
	class CBaseHandle;

	class IClientEntityList {
	public:
		// get IClientNetworkable interface for specified entity
		virtual void* get_client_networkable( int ent_num ) = 0;
		virtual void* get_client_networkable_from_handle( CBaseHandle ent ) = 0;
		virtual void* get_client_unknown_from_handle( CBaseHandle ent ) = 0;

		// note: this function is only a convenience wrapper.
		// it returns GetClientNetworkable( entnum )->GetIClientEntity().
		virtual void* get_client_entity( int ent_num ) = 0;
		virtual void* get_client_entity_from_handle( CBaseHandle ent ) = 0;

		// returns number of entities currently in use
		virtual int	number_of_entities( bool include_non_networkable ) = 0;

		// returns highest index actually used
		virtual int	get_highest_entity_index( void ) = 0;

		// sizes entity list to specified size
		virtual void set_max_entities( int max_ents ) = 0;
		virtual int	get_max_entities( void ) = 0;


		char _pad0[ 0x12 ];
		// current count
		int m_num_server_ents;
		// max allowed
		int m_max_server_ents;
		int m_num_clients_not_networked;
		// current last used slot
		int m_max_server_used_index;
	};
}