#include "math.h"

namespace n_math {
    angle_t calc_angle( const vec3_t& src, const vec3_t& dst ) {
        auto angle = angle_t();
        const vec3_t delta = src - dst;
        vector_angles( delta, angle );
        return angle.clamped();
    }

    void angle_vectors( const angle_t& angle, vec3_t& out ) {
        vec2_t
            sine = { },
            cosine = { };

        sincos( deg2rad( angle.x ), sine.x, cosine.x );
        sincos( deg2rad( angle.y ), sine.y, cosine.y );

        out.x = cosine.x * cosine.y;
        out.y = cosine.x * sine.y;
        out.z = -sine.x;
    }

    void angle_vectors( const angle_t& angle, vec3_t& out_forward, vec3_t& out_right, vec3_t& out_up ) {
        vec3_t
            sine = { },
            cosine = { };

        sincos( deg2rad( angle.x ), sine.x, cosine.x );
        sincos( deg2rad( angle.y ), sine.y, cosine.y );
        sincos( deg2rad( angle.z ), sine.z, cosine.z );

        out_forward.x = cosine.x * cosine.y;
        out_forward.y = cosine.x * sine.y;
        out_forward.z = -sine.x;

        out_right.x = ( -1 * sine.z * sine.x * cosine.y + -1 * cosine.z * -sine.y );
        out_right.y = ( -1 * sine.z * sine.x * sine.y + -1 * cosine.z * cosine.y );
        out_right.z = ( -1 * sine.z * cosine.x );

        out_up.x = ( cosine.z * sine.x * cosine.y + -sine.z * -sine.y );
        out_up.y = ( cosine.z * sine.x * sine.y + -sine.z * cosine.y );
        out_up.z = ( cosine.z * cosine.x );
    }

    vec3_t angle_vectors( const angle_t & angle ) {
        vec3_t out{ };
        angle_vectors( angle, out );
        return out;
    }

    void vector_angles( const vec3_t & forward, angle_t & out ) {
        if ( forward.x == 0.0f && forward.y == 0.0f ) {
            out.x = ( forward.z > 0.0f ) ? 270.0f : 90.0f;
            out.y = 0.0f;
        }
        else {
            out.x = atan2( -forward.z, forward.length_2d() ) * ( -180.f / static_cast<float>( M_PI ) );
            out.y = atan2( forward.y, forward.x ) * ( 180.f / static_cast<float>( M_PI ) );

            if ( out.y > 90.f )
                out.y -= 180.f;
            else if ( out.y < 90.f )
                out.y += 180.f;
            else if ( out.y == 90.f )
                out.y = 0.f;
        }

        out.z = 0.f;
    }

    angle_t vector_angles( const vec3_t & forward ) {
        angle_t out{ };
        vector_angles( forward, out );
        return out;
    }

    vec3_t vector_transform( vec3_t & in, matrix3x4_t & matrix ) {
        return vec3_t(
            in.dot( matrix[0] ) + matrix[0][3],
            in.dot( matrix[1] ) + matrix[1][3],
            in.dot( matrix[2] ) + matrix[2][3] );
    }

    float fov( const angle_t & view_angles, const angle_t & aim_angles, const float distance ) {
        angle_t delta = ( view_angles - aim_angles ).normalized();

        if ( std::fabsf( delta.y ) > 90.f )
            return 400.f; // more than 90°

        delta.x = sin( deg2rad( delta.x ) ) * distance;
        delta.y = sin( deg2rad( delta.y ) ) * distance;
        return delta.length_2d();
    }

}