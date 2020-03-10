#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/ihandleentity.h

namespace n_sdk {
	class CBaseHandle;

	class IHandleEntity {
	public:
		virtual ~IHandleEntity( void ) { }

		virtual void set_handle( const CBaseHandle& handle ) = 0;

		virtual const CBaseHandle& get_handle( ) const = 0;
	};
}