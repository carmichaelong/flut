#pragma once

#include <cmath>

#include "flut/system/types.hpp"

namespace flut
{
	namespace math
	{
		template< typename T > struct vec4_
		{
			vec4_() {}
			vec4_( T px, T py, T pz, T pw ) : x( px ), y( py ), z( pz ), w( pw ) {}

			// element access
			const T& operator[]( index_t idx ) const { return (&x)[idx]; }
			T& operator[]( index_t idx ) { return (&x)[idx]; }
			T x, y, z, w;

			// const properties
			T length() const { return sqrt( x * x + y * y + z * z + w * w ); }
			T squared_length() const { return x * x + y * y + z * z + w * w; }
			bool is_null() const { return x == T(0) && y == T(0) && z == T(0) && w == T(0); }

			// static initializers
			static vec4_<T> zero() { return vec4_<T>( T(0), T(0), T(0), T(0) ); }
			static vec4_<T> unit_x() { return vec4_<T>( T(1), T(0), T(0), T(0) ); }
			static vec4_<T> unit_y() { return vec4_<T>( T(0), T(1), T(0), T(0) ); }
			static vec4_<T> unit_z() { return vec4_<T>( T(0), T(0), T(1), T(0) ); }
			static vec4_<T> unit_w() { return vec4_<T>( T(0), T(0), T(0), T(1) ); }
		};

		/// template instantiations
		typedef vec4_< real_t > vec4;
		typedef vec4_< float > vec4f;
		typedef vec4_< double > vec4d;
	}
}
