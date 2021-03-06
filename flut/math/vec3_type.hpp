#pragma once

#include <cmath>
#include "flut/system/types.hpp"
#include "flut/prop_node.hpp"

namespace flut
{
	namespace math
	{
		template< typename T > struct vec3_
		{
			typedef T* iterator;
			typedef const T* const_iterator;

			vec3_() {}
			vec3_( T px, T py, T pz ) : x( px ), y( py ), z( pz ) {}
			vec3_( T v ) : x( v ), y( v ), z( v ) {}
			template< typename T2 > vec3_( const vec3_<T2>& o ) : x( T(o.x) ), y( T(o.y) ), z( T(o.z) ) {}
			template< typename T2 > vec3_<T>& operator=( const vec3_<T2>& o ) { x = T(o.x); y = T(o.y); z = T(o.z); return *this; }
			vec3_( const prop_node& pn ) : x( pn.get<T>( "x", T(0) ) ), y( pn.get<T>( "y", T(0) ) ), z( pn.get<T>( "z", T(0) ) ) {}

			/// convert to prop_node
			explicit operator prop_node() const { return prop_node().set( "x", x ).set( "y", y ).set( "z", z ); }

			/// element access
			const T& operator[]( index_t idx ) const { return (&x)[idx]; }
			T& operator[]( index_t idx ) { return (&x)[idx]; }
			iterator begin() { return &x; }
			iterator end() { return &x + 3; }
			const_iterator begin() const { return &x; }
			const_iterator end() const { return &x + 3; }
			T x, y, z;

			/// const properties
			T length() const { return sqrt( x * x + y * y + z * z ); }
			T squared_length() const { return x * x + y * y + z * z; }
			bool is_null() const { return x == T(0) && y == T(0) && z == T(0); }
			void clear() { x = y = z = T(0); }

			/// static initializers
			static vec3_<T> zero() { return vec3_<T>( T(0), T(0), T(0) ); }
			static vec3_<T> unit_x() { return vec3_<T>( T(1), T(0), T(0) ); }
			static vec3_<T> unit_y() { return vec3_<T>( T(0), T(1), T(0) ); }
			static vec3_<T> unit_z() { return vec3_<T>( T(0), T(0), T(1) ); }
			static vec3_<T> neg_unit_x() { return vec3_<T>( T(-1), T(0), T(0) ); }
			static vec3_<T> neg_unit_y() { return vec3_<T>( T(0), T(-1), T(0) ); }
			static vec3_<T> neg_unit_z() { return vec3_<T>( T(0), T(0), T(-1) ); }
			static vec3_<T> uni( T v ) { return vec3_<T>( v, v, v ); }
		};

		/// template instantiations
		typedef vec3_< real_t > vec3;
		typedef vec3_< float > vec3f;
		typedef vec3_< double > vec3d;
	}

	template< typename T > struct prop_node_cast< math::vec3_<T> > {
		static math::vec3_<T> from( const prop_node& pn ) { return math::vec3_<T>( pn ); }
		static prop_node to( const math::vec3_<T>& vec ) { return static_cast< prop_node >( vec ); }
	};
}
