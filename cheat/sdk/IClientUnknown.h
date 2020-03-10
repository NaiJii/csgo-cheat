#pragma once

#include "IHandleEntity.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iclientunknown.h

namespace n_sdk {
	class C_BaseEntity;

	class IClientUnknown : public IHandleEntity {
	public:
		virtual void* get_collideable( void ) = 0;
		virtual void* get_client_networkable( void ) = 0;
		virtual void* get_client_renderable( void ) = 0;
		virtual void* get_client_entity( void ) = 0;
		virtual C_BaseEntity* get_base_entity( void ) = 0;
		virtual void* get_client_thinkable( void ) = 0;
		virtual void* get_client_alpha_property( void ) = 0;
	};
}