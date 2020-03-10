#pragma once

// https://github.com/VSES/SourceEngine2007/blob/master/se2007/engine/client.h

namespace n_sdk {
	class INetChannel;
	class CEventInfo;

	class CClientState {
	public:
		char _pad0[ 0x9C ]; // 0
		INetChannel* m_net_channel; // 156
		char _pad1[ 0x68 ]; // 160
		int m_sign_on_state; // 264
		char _pad2[ 0x4C18 ]; // 268
		int m_last_outgoing_command; // 19,748 
		int m_chocked_commands; // 19,752 
		char _pad3[ 0x138 ]; // 19,756
		CEventInfo* m_events; // 20,068
	};
}