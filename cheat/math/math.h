#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>

#include "vector2d.h"
#include "vector3d.h"
#include "matrix3x4_t.h"
#include "qangle.h"
#include "color.h"

namespace n_math {
    ///============================= Definitions =============================///
    inline float rad2deg( const float rad ) {
        return static_cast<float>( rad * ( 180.f / M_PI ) );
    }
    inline float deg2rad( const float deg ) {
        return static_cast<float>( deg * ( M_PI / 180.f ) );
    }
    inline float random_float( const float min, const float max ) {
        static auto RandomFloat_fn = reinterpret_cast<float( * )( float, float )>( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomFloat" ) );
        return RandomFloat_fn( min, max );
    }
    inline void sincos( const float rad, float& sine, float& cosine ) {
        sine = std::sin( rad );
        cosine = std::cos( rad );
    }
    ///========================================================================///
    angle_t calc_angle( const vec3_t & src, const vec3_t & dst );
    void angle_vectors( const angle_t & angle, vec3_t & out );
    void angle_vectors( const angle_t & angle, vec3_t & out_forward, vec3_t & out_right, vec3_t & out_up );
    vec3_t angle_vectors( const angle_t & angle );
    void vector_angles( const vec3_t & forward, angle_t & out );
    angle_t vector_angles( const vec3_t & forward );
    vec3_t vector_transform( vec3_t & in, matrix3x4_t & matrix );
    float fov( const angle_t & view_angles, const angle_t & aim_angles, const float distance );
}