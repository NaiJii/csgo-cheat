#pragma once

#include "vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/mathlib/mathlib.h

using matrix4x4_t = float[ 4 ][ 4 ];

struct matrix3x4_t {
	matrix3x4_t( void ) { };

	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 ) {
		this->m_mat_val[ 0 ][ 0 ] = m00;
		this->m_mat_val[ 0 ][ 1 ] = m01;
		this->m_mat_val[ 0 ][ 2 ] = m02;
		this->m_mat_val[ 0 ][ 3 ] = m03;
		this->m_mat_val[ 1 ][ 0 ] = m10;
		this->m_mat_val[ 1 ][ 1 ] = m11;
		this->m_mat_val[ 1 ][ 2 ] = m12;
		this->m_mat_val[ 1 ][ 3 ] = m13;
		this->m_mat_val[ 2 ][ 0 ] = m20;
		this->m_mat_val[ 2 ][ 1 ] = m21;
		this->m_mat_val[ 2 ][ 2 ] = m22;
		this->m_mat_val[ 2 ][ 3 ] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init( const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin ) {
		this->m_mat_val[ 0 ][ 0 ] = x_axis.x;
		this->m_mat_val[ 0 ][ 1 ] = y_axis.x;
		this->m_mat_val[ 0 ][ 2 ] = z_axis.x;
		this->m_mat_val[ 0 ][ 3 ] = vec_origin.x;
		this->m_mat_val[ 1 ][ 0 ] = x_axis.y;
		this->m_mat_val[ 1 ][ 1 ] = y_axis.y;
		this->m_mat_val[ 1 ][ 2 ] = z_axis.y;
		this->m_mat_val[ 1 ][ 3 ] = vec_origin.y;
		this->m_mat_val[ 2 ][ 0 ] = x_axis.z;
		this->m_mat_val[ 2 ][ 1 ] = y_axis.z;
		this->m_mat_val[ 2 ][ 2 ] = z_axis.z;
		this->m_mat_val[ 2 ][ 3 ] = vec_origin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t( const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin ) {
		init( x_axis, y_axis, z_axis, vec_origin );
	}

	void invalidate( void ) {
		for ( auto& i : this->m_mat_val )
			for ( float& j : i )
				j = std::numeric_limits< float >::infinity( );
	}

	float* operator[]( int i ) {
		return this->m_mat_val[ i ];
	}

	const float* operator[]( int i ) const {
		return this->m_mat_val[ i ];
	}

	float* base( ) {
		return &this->m_mat_val[ 0 ][ 0 ];
	}

	const float* base( ) const {
		return &this->m_mat_val[ 0 ][ 0 ];
	}

	float m_mat_val[ 3 ][ 4 ];
};
