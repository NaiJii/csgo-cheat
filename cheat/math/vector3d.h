#pragma once

#include <cmath>
#include <algorithm>

struct vec3_t {
    vec3_t() = default;
    vec3_t( const float _x, const float _y ) : x( _x ), y( _y ), z( 0 ) { };
    vec3_t( const float _x, const float _y, const float _z ) : x( _x ), y( _y ), z( _z ) { };
    vec3_t( const float* arr ) : x( arr[0] ), y( arr[1] ), z( arr[2] ) { };

    inline float dot( const vec3_t& point ) {
        return ( x * point.x + y * point.y + z * point.z );
    }

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

    void zero( void ) {
        x = y = z = 0.0f;
    }

    inline vec3_t normalize( void ) {
        vec3_t res = *this;
        float l = res.length();
        if ( l != 0.0f )
            res /= l;
        else
            res.x = res.y = res.z = 0.0f;
        return res;
    }

    inline float distance_to( vec3_t point ) {
        vec3_t delta = { *this - point };

        return delta.length();
    }

    inline vec3_t operator+( vec3_t point ) const {
        return vec3_t( x + point.x, y + point.y, z + point.z );
    }

    inline vec3_t operator-( vec3_t point ) const {
        return vec3_t( x - point.x, y - point.y, z - point.z );
    }

    inline vec3_t operator*( const float point ) const {
        return vec3_t( x * point, y * point, z * point );
    }

    inline vec3_t& operator=( const float array[3] ) {
        x = array[0];
        y = array[1];
        z = array[2];
        return *this;
    }

    inline vec3_t& operator+=( vec3_t point ) {
        x += point.x;
        y += point.y;
        z += point.z;
        return *this;
    }

    inline vec3_t& operator-=( vec3_t point ) {
        x -= point.x;
        y -= point.y;
        z -= point.z;
        return *this;
    }

    inline vec3_t& operator*=( const float multiplier ) {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        return *this;
    }

    inline vec3_t& operator/=( const float division ) {
        x /= division;
        y /= division;
        z /= division;
        return *this;
    }

    float x, y, z;
};