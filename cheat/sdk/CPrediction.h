#pragma once

#include "../math/vector3d.h"
#include "../math/qangle.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/prediction.h

namespace n_sdk {
	class C_BasePlayer;
	class CUserCmd;
	class IMoveHelper;
	class CMoveData;

	class CPrediction {
	public:
		// construction
		virtual ~CPrediction( void ) = 0;
		virtual void init( void ) = 0;
		virtual void shutdown( void ) = 0;

		// implement IPrediction
		virtual void update( int startframe, bool validframe, int incoming_acknowledged, int outgoing_command ) = 0;
		virtual void pre_entity_packet_received( int commands_acknowledged, int current_world_update_packet, int unknown ) = 0;
		virtual void post_entity_packet_received( void ) = 0;
		virtual void post_network_data_received( int commands_acknowledged ) = 0;
		virtual void on_received_uncompressed_packet( void ) = 0;

		// the engine needs to be able to access a few predicted values
		virtual void get_view_origin( vec3_t& org ) = 0;
		virtual void set_view_origin( vec3_t& org ) = 0;
		virtual void get_view_angles( angle_t& ang ) = 0;
		virtual void set_view_angles( angle_t& ang ) = 0;
		virtual void get_local_view_angles( angle_t& ang ) = 0;
		virtual void set_local_view_angles( angle_t& ang ) = 0;

		// implement IPrediction
		virtual bool in_prediction( void ) const = 0;
		virtual bool is_first_time_predicted( void ) const = 0;
		virtual int get_last_acknowledged_command_number( void ) const = 0;
		virtual int	get_incoming_packet_number( void ) const = 0;
		virtual void check_moving_ground( C_BasePlayer* player, double frametime ) = 0;
		virtual void run_command( C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* move_helper ) = 0;

		// internal
		virtual void setup_move( C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* helper, CMoveData* move ) = 0;
		virtual void finish_move( C_BasePlayer* player, CUserCmd* cmd, CMoveData* move ) = 0;
		virtual void set_ideal_pitch( int slot, C_BasePlayer* player, const vec3_t& origin, const vec3_t& angles, const vec3_t& viewheight ) = 0;
		virtual void check_error( int slot, C_BasePlayer* player, int commands_acknowledged ) = 0;
		virtual void _update( int nSlot, bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command ) = 0;

		char _pad0[ 0x08 ];
		bool m_in_prediction; // set this shit to true https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/shared/baseplayer_shared.cpp#L674-L676
		char _pad1[ 0x1 ];
		bool m_engine_paused; 
		char _pad2[ 0xD ];
		bool m_is_first_time_predicted; // set this shit to false https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/shared/baseplayer_shared.cpp#L674-L676
	};
}