#pragma once

#include "../math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/client_class.h

namespace n_sdk {
	class IClientNetworkable;
	class RecvProp;

	// the serial number that gets passed in is used for ehandles.
	using CreateClientClassFn = IClientNetworkable*( * )( int, int );
	using CreateEventFn = IClientNetworkable*( * )( );

	struct DVariant {
		union {
			float m_float;
			long m_int;
			char* m_string;
			void* m_data;
			vec3_t m_vector;
			int64_t m_int_64;
		};

		int m_type;
	};

	struct CRecvProxyData {
		const RecvProp* m_recv_prop;
		DVariant m_value;
		int m_element;
		int m_object_id;
	};

	using RecvVarProxyFn = void( __cdecl* )( CRecvProxyData*, void*, void* );

	enum SendPropType : int {
		DPT_Int = 0,
		DPT_Float,
		DPT_Vector,
		DPT_VectorXY,
		DPT_String,
		DPT_Array,
		DPT_DataTable,
		DPT_Int64,
		DPT_NUMSendPropTypes
	};

	class RecvTable {
	public:
		RecvProp* m_prop;
		int m_props;
		void* m_decoder;
		char* m_net_table_name;
		bool m_initialized;
		bool m_in_main_list;
	};

	class RecvProp {
	public:
		char* m_var_name;
		SendPropType m_recv_type;
		int m_flags;
		int m_string_buffer_size;
		bool m_inside_array;
		const void* m_extra_data;
		RecvProp* m_array_prop;
		void* m_array_length_proxy;
		RecvVarProxyFn m_proxy;
		void* m_data_table_proxy;
		RecvTable* m_data_table;
		int m_offset;
		int m_element_stride;
		int m_elements;
		const char* m_parent_array_prop_name;
	};

	class ClientClass {
	public:
		CreateClientClassFn m_create;
		// only called for event objects.
		CreateEventFn m_create_event;
		char* m_network_name;
		RecvTable* m_table;
		ClientClass* m_next;
		// managed by the engine.
		int m_class_id;
	};
}