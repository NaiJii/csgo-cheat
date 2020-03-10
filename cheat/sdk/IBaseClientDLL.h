#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

namespace n_sdk {
	class ClientClass;

	enum ClientFrameStage_t {
		// havent run any frame
		FRAME_UNDEFINED = -1,
		FRAME_START,

		// anetwork packet is being recieved
		FRAME_NET_UPDATE_START,
		// fata has been received and we're going to start calling PostDataUpdate
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		// fata has been received and we've called PostDataUpdate on all data recipients
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		// we've received all packets, we can now do interpolation, prediction, etc..
		FRAME_NET_UPDATE_END,

		// we're about to start rendering the scene
		FRAME_RENDER_START,
		// we've finished rendering the scene.
		FRAME_RENDER_END
	};

	class IBaseClientDLL {
	public:
		// request a pointer to the list of client datatable classes
		virtual_fn( get_all_classes( ), 8, ClientClass* ( __thiscall* )( void* ) );
	};
}