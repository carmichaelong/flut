#include "char_stream.hpp"
#include "string_tools.hpp"
#include <cstring>

namespace flut
{

	char_stream::char_stream()
	{
		init_buffer( str_buffer.c_str() );
	}

	char_stream::char_stream( const char* buf )
	{
		init_buffer( buf );
	}

	char_stream::char_stream( string&& other ) : str_buffer( std::move( other ) )
	{
		init_buffer( str_buffer.c_str() );
	}

	flut::char_stream& char_stream::operator>>( string& s )
	{
		for ( end_pos = const_cast<char*>( cur_pos ); *end_pos && !isspace( *end_pos ); ++end_pos );
		s = string( cur_pos, size_t( end_pos - cur_pos ) );
		process_end_pos();
		return *this;
	}

	flut::string char_stream::get_line()
	{
		size_t len = strcspn( cur_pos, "\r\n" );
		string s = string( cur_pos, len );
		end_pos = const_cast<char*>(cur_pos)+len;
		process_end_pos();
		return s;
	}

	void char_stream::init_buffer( const char* b )
	{
		flut_assert( b != 0 );
		cur_pos = buffer = b;
		end_pos = nullptr;
		skip_whitespace();
	}

	flut::char_stream load_char_buffer( const string& filename )
	{
		return char_stream( load_string( filename ) );
	}

}