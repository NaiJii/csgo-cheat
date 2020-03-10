#pragma once

#include <cmath>
#include <algorithm>

struct color_t {
    int r, g, b, a;

    color_t() = default;

    color_t( const int r, const int g, const int b ) {
        this->r = r, this->g = g, this->b = b, this->a = 255;
    }

    color_t( const int r, const int g, const int b, const int a ) {
        this->r = r, this->g = g, this->b = b, this->a = a;
    }

    color_t operator+( const color_t& color ) const {
        return color_t( this->r + color.r, this->g + color.g, this->b + color.b, this->a + color.a );
    }

    color_t operator-( const color_t & color ) const {
        return color_t( this->r - color.r, this->g - color.g, this->b - color.b, this->a - color.a );
    }

    color_t operator*( const int value ) const {
        return color_t( this->r * value, this->g * value, this->b * value, this->a * value );
    }

    color_t operator/( const int value ) const {
        return color_t( this->r / value, this->g / value, this->b / value, this->a / value );
    }

    color_t& operator=( const int array[4] ) {
        this->r = array[0];
        this->g = array[1];
        this->b = array[2];
        this->a = array[4];

        return *this;
    }

    color_t& operator+=( const color_t & color ) {
        this->r += color.r;
        this->g += color.g;
        this->b += color.b;
        this->a += color.a;

        return *this;
    }

    color_t& operator-=( const color_t & color ) {
        this->r -= color.r;
        this->g -= color.g;
        this->b -= color.b;
        this->a -= color.a;

        return *this;
    }

    color_t& operator*=( const color_t & color ) {
        this->r *= color.r;
        this->g *= color.g;
        this->b *= color.b;
        this->a *= color.a;

        return *this;
    }

    color_t& operator/=( const int division ) {
        this->r /= division;
        this->g /= division;
        this->b /= division;
        this->a /= division;

        return *this;
    }
};