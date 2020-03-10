#pragma once
#include <cmath>
#include <algorithm>

struct vec2_t {
    float x, y;

    vec2_t() = default;

    vec2_t( const float _x, const float _y ) {
        x = _x; y = _y;
    };

    inline vec2_t operator*( const float n ) const {
        return vec2_t( x * n, y * n );
    }

    inline vec2_t operator/( const float n ) const {
        return vec2_t( x / n, y / n );
    }

    inline vec2_t operator+( const vec2_t & v ) const {
        return vec2_t( x + v.x, y + v.y );
    }

    inline vec2_t operator-( const vec2_t & v ) const {
        return vec2_t( x - v.x, y - v.y );
    }

    inline void operator+=( const vec2_t & v ) {
        x += v.x;
        y += v.y;
    }

    inline void operator-=( const vec2_t & v ) {
        x -= v.x;
        y -= v.y;
    }

    bool operator==( const vec2_t & v ) const {
        return ( v.x == x && v.y == y );
    }
    bool operator!=( const vec2_t & v ) const {
        return ( v.x != x || v.y != y );
    }

    inline float length() {
        return std::sqrt( x * x + y * y );
    }

    inline float distance_to( const vec2_t & v ) {
        return sqrt( ( v.x - x ) * ( v.x - x ) + ( v.y - y ) * ( v.y - y ) );
    }
};