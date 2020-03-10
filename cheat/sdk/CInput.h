#pragma once

#include "../math/vector3d.h"
#include "../math/qangle.h"

class CUserCmd;

namespace n_sdk {
    class CInput {
    public:
        char pad0[0x0C];
        bool trackir_available;
        bool mouse_initiated;
        bool mouse_active;
        bool joystick_advanced_init;
        char pad1[0x2C];
        void* keys;
        char pad2[0x6C];
        bool camera_intercepting_mouse;
        bool camera_in_third_person;
        bool camera_moving_with_mouse;
        vec3_t vec_camera_offset;
        bool camera_distance_move;
        int camera_old_x;
        int camera_old_y;
        int camera_x;
        int camera_y;
        bool camera_is_orthographic;
        angle_t previous_view_angles;
        angle_t previous_view_angles_tilt;
        float last_forward_move;
        int clear_input_state;
        char pad3[0x8];
        CUserCmd* commands;
    };
}