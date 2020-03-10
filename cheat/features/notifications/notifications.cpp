#include "notifications.h"

#include "../../interfaces/interfaces.h"

namespace n_features {
	c_notification::c_notification( const std::string& notification ) {
		this->m_spawn_time = n_interfaces::global_vars_base->current_time;
		this->m_message = notification;
	}
}