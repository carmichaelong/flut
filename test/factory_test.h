#pragma once

#include "flut/prop_node.hpp"
#include "flut/factory.hpp"
#include "flut/system/test_framework.hpp"

namespace flut
{
	struct A { A( const prop_node& pn ) : value( 0 ) {} int value; };
	struct B : A { B( const prop_node& pn ) : A( pn ) { value = 67; } };
	struct C : A { C( const prop_node& pn ) : A( pn ) { value = -1; } };

	void factory_test()
	{
		factory< A, prop_node > fact;
		fact.register_class< B >( "B" );
		fact.register_class< C >( "C" );

		prop_node pn;

		auto b = fact( "B" )( pn );
		auto c = fact( "C" )( pn );

		FLUT_TEST( b->value == 67 );
		FLUT_TEST( c->value == -1 );
	}
}
