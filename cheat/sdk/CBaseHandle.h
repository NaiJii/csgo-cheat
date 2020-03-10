#pragma once

#include "IHandleEntity.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/basehandle.h

#define NUM_ENT_ENTRY_BITS         ( 11 + 2 )
#define NUM_ENT_ENTRIES            ( 1 << NUM_ENT_ENTRY_BITS )
#define INVALID_EHANDLE_INDEX      0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS        16
#define NUM_SERIAL_NUM_SHIFT_BITS  ( 32 - NUM_SERIAL_NUM_BITS )
#define ENT_ENTRY_MASK             ( ( 1 << NUM_SERIAL_NUM_BITS ) - 1)

namespace n_sdk {
	class CBaseHandle {
	public:
		CBaseHandle( void );
		CBaseHandle( const CBaseHandle& other );
		CBaseHandle( unsigned long value );
		CBaseHandle( int entry, int serial_number );

		void init( int entry, int serial_number );
		void term( void );

		// even if this returns true, Get() still can return return a non-null value.
		// this just tells if the handle has been initted with any values.
		bool is_valid( void ) const;

		int get_entry_index( void ) const;
		int get_serial_number( void ) const;

		int to_int( void ) const;
		bool operator !=( const CBaseHandle& other ) const;
		bool operator ==( const CBaseHandle& other ) const;
		bool operator ==( const IHandleEntity* ent ) const;
		bool operator !=( const IHandleEntity* ent ) const;
		bool operator <( const CBaseHandle& other ) const;
		bool operator <( const IHandleEntity* ent ) const;

		// assign a value to the handle.
		const CBaseHandle& operator=( const IHandleEntity* entity );
		const CBaseHandle& set( const IHandleEntity* entity );

		IHandleEntity* get( void ) const;

	protected:
		unsigned long index;
	};

	inline CBaseHandle::CBaseHandle( void ) {
		index = INVALID_EHANDLE_INDEX;
	}

	inline CBaseHandle::CBaseHandle( const CBaseHandle& other ) {
		index = other.index;
	}

	inline CBaseHandle::CBaseHandle( unsigned long value ) {
		index = value;
	}

	inline CBaseHandle::CBaseHandle( int entry, int serial_number ) {
		init( entry, serial_number );
	}

	inline void CBaseHandle::init( int entry, int serial_number ) {
		index = static_cast< unsigned long >( entry | ( serial_number << NUM_SERIAL_NUM_SHIFT_BITS ) );
	}

	inline void CBaseHandle::term( void ) {
		index = INVALID_EHANDLE_INDEX;
	}

	inline bool CBaseHandle::is_valid( void ) const {
		return index != INVALID_EHANDLE_INDEX;
	}

	inline int CBaseHandle::get_entry_index( void ) const {
		if ( !is_valid( ) )
			return NUM_ENT_ENTRIES - 1;

		return index & ENT_ENTRY_MASK;
	}

	inline int CBaseHandle::get_serial_number( void ) const {
		return index >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	inline int CBaseHandle::to_int( void ) const {
		return static_cast< int >( index );
	}

	inline bool CBaseHandle::operator !=( const CBaseHandle& other ) const {
		return index != other.index;
	}

	inline bool CBaseHandle::operator ==( const CBaseHandle& other ) const {
		return index == other.index;
	}

	inline bool CBaseHandle::operator ==( const IHandleEntity* ent ) const {
		return get( ) == ent;
	}

	inline bool CBaseHandle::operator !=( const IHandleEntity* ent ) const {
		return get( ) != ent;
	}

	inline bool CBaseHandle::operator <( const CBaseHandle& other ) const {
		return index < other.index;
	}

	inline bool CBaseHandle::operator <( const IHandleEntity* entity ) const {
		unsigned long otherIndex = ( entity ) ? entity->get_handle( ).index : INVALID_EHANDLE_INDEX;
		return index < otherIndex;
	}

	inline const CBaseHandle& CBaseHandle::operator=( const IHandleEntity* entity ) {
		return set( entity );
	}

	inline const CBaseHandle& CBaseHandle::set( const IHandleEntity* entity ) {
		if ( entity )
			*this = entity->get_handle( );
		else
			index = INVALID_EHANDLE_INDEX;

		return *this;
	}
}