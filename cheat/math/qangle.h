#pragma once

#include <cmath>
#include <algorithm>

struct angle_t {
    float x, y, z;

    angle_t() = default;

    angle_t( const float _x, const float _y, const float _z ) : x( _x ), y( _y ), z( _z ) { };
    angle_t( const float arr[3] ) : x( arr[0] ), y( arr[1] ), z( arr[2] ) { };

    inline float length( void ) const {
        return sqrtf( x * x + y * y + z * z );
    }

    inline float length_sqr( void ) const {
        return ( x * x + y * y + z * z );
    }

    inline float length_2d( void ) const {
        float root = 0.f;
        float sqst = x * x + y * y;
        __asm
        {
            sqrtss xmm0, sqst
            movss root, xmm0
        }
        return root;
    }

    inline angle_t operator+( const angle_t & point ) const {
        return angle_t( x + point.x, y + point.y, z + point.z );
    }

    inline angle_t operator-( const angle_t & point ) const {
        return angle_t( x - point.x, y - point.y, z - point.z );
    }

    inline angle_t operator*( const float point ) const {
        return angle_t( x * point, y * point, z * point );
    }

    inline void operator+=( const angle_t & point ) {
        x += point.x;
        y += point.y;
        z += point.z;
    }

    inline void operator-=( const angle_t & point ) {
        x -= point.x;
        y -= point.y;
        z -= point.z;
    }

    inline void operator/=( const float division ) {
        x /= division;
        y /= division;
        z /= division;
    }

    inline void operator*=( const float multiplier ) {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
    }

    void normalize( void ) {
        x = std::isfinite( x ) ? std::remainderf( x, 360.0f ) : 0.0f;
        y = std::isfinite( y ) ? std::remainderf( y, 360.0f ) : 0.0f;
        z = 0.0f;
    }

    angle_t& normalized( void ) {
        angle_t out = *this;
        out.normalize();
        return out;
    }

    inline void clamp( void ) {
        this->normalize();

        x = std::clamp( x, -89.0f, 89.0f );
        y = std::clamp( y, -180.0f, 180.0f );
        z = 0.0f;
    }

    inline angle_t& clamped( void ) {
        angle_t out = {};
        out.x = std::clamp( x, -89.0f, 89.0f );
        out.y = std::clamp( y, -180.0f, 180.0f );
        out.z = 0.f;
        return out;
    }
};