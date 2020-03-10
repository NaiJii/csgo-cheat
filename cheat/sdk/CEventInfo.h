#pragma once

namespace n_sdk {
	class ClientClass;

	class CEventInfo {
	public:
		char _pad0[ 0x4 ]; // 0
		float m_delay; // 4
		char _pad1[ 0x4 ]; // 8
		ClientClass* m_client_class; // 12
		char _pad2[ 0x28 ]; // 16
		CEventInfo* m_next_object; // 56
	};
} 