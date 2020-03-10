#pragma once

#include <string>
#include <cstdint>

using hash_t = uint32_t;

namespace n_hash {
    constexpr hash_t BASIS = 0x811c9dc5;
    constexpr hash_t PRIME = 0x1000193;

    template <hash_t N>
    inline constexpr hash_t ct_hash( const char( &str )[N], hash_t Len = N - 1 ) {
        return ( Len <= 1 ) ? str[0] : ( PRIME * ct_hash( PRIME, str, Len - 1 ) + str[Len - 1] );
    }

    inline hash_t rt_hash( const char* str ) {
        hash_t ret = BASIS;
        hash_t length = strlen( str );

        for ( auto i = 0u; i < length; ++i ) {
            ret ^= str[i];
            ret *= PRIME;
        }

        return ret;
    }
}

/// Compile Time Hashing
#define _cth(str) (n_hash::ct_hash(str))
/// Run Time Hashing
#define _rth(str) (n_hash::rt_hash(str))

