#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetchannelinfo.h

namespace n_sdk {
	class INetChannelInfo {
	public:
		// current latency (RTT), more accurate but jittering
		virtual_fn( get_latency( int flow ), 9, float( __thiscall* )( void*, int ), flow );

		// average packet latency in seconds
		virtual_fn( get_average_latency( int flow ), 10, float( __thiscall* )( void*, int ), flow );
	};
}