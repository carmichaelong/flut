#include "string_tools.hpp"

#ifdef FLUT_COMP_MSVC
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	include <shlwapi.h> // used by glob_match
#	undef small // windows defines small :-S
#	pragma comment( lib, "shlwapi.lib" )
#else
#	include <fnmatch.h>
#endif

#include <stdarg.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "error_code.h"

namespace flut
{
	int to_str_precision_value = 4;

	string trim_str( const string& s )
	{
		const char* space_chars = " \t\r\n\f\v";
		auto left = s.find_first_not_of( space_chars );
		if ( left == string::npos ) return string( "" ); // string has no non-whitespace characters
		auto right = s.find_last_not_of( space_chars );
		return s.substr( left, 1 + right - left );
	}

	string trim_right_str( const string& s )
	{
		const char* space_chars = " \t\r\n\f\v";
		auto right = s.find_last_not_of( space_chars );
		return s.substr( 0, 1 + right );
	}

	vector< string > split_str( const string& s, const string& sep_chars )
	{
		std::vector< string > strings;
		size_t ofs = s.find_first_not_of( sep_chars.c_str(), 0 );
		while ( ofs != string::npos ) {
			size_t ofsend = s.find_first_of( sep_chars.c_str(), ofs );
			strings.push_back( s.substr( ofs, ofsend - ofs ) );
			ofs = s.find_first_not_of( sep_chars.c_str(), ofsend );
		}
		return strings;
	}

	FLUT_API std::pair< string, string > key_value_str( const string& s, const string& sep_char )
	{
		auto pos = s.find_first_of( sep_char.c_str() );
		if ( pos == string::npos )
			return make_pair( s, string("") );
		else return make_pair( trim_str( s.substr( 0, pos ) ), trim_str( mid_str( s, pos + 1 ) ) );
	}

	string stringf( const char* format, ... )
	{
#ifdef FLUT_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4996 )
#endif
		va_list args;
		va_start( args, format );
		char buf[256];
		vsnprintf( buf, sizeof( buf ), format, args );
		va_end( args );
		return string( buf );
#ifdef FLUT_COMP_MSVC
#	pragma warning( pop )
#endif
	}

	bool pattern_match( const string& str, const string& pattern )
	{
#ifdef FLUT_COMP_MSVC
		flut_assert_msg( str.find_first_of( ";" ) == string::npos, "pattern_match patterns cannot contain ';' on MSVC" );
		return PathMatchSpecEx( str.c_str(), pattern.c_str(), PMSF_NORMAL ) == S_OK;
#else
		return fnmatch( pattern.c_str(), str.c_str(), FNM_NOESCAPE ) == 0;
#endif
	}

	FLUT_API int set_to_str_precision( int p )
	{ int old = to_str_precision_value; to_str_precision_value = p; return old; }

	FLUT_API int to_str_precision()
	{ return to_str_precision_value; }

	string load_string( const path& filename, error_code* ec )
	{
		// this method uses a stringbuf, which may be slower but more stable
		std::ifstream ifstr( filename.str() );
		if ( !ifstr.good() )
		{
			if ( try_set_error( ec, "Could not open " + filename.str() ) )
				return "";
			else flut_error( "Could not open " + filename.str() );
		}
		std::stringstream buf;
		buf << ifstr.rdbuf();
		return buf.str();
	}

	FLUT_API string encode_char( char c )
	{
		if ( c == '\"' )
			return "\\\"";
		else if ( c == '\\' )
			return "\\\\";
		else if ( c < 32 )
		{
			switch ( c )
			{
			case '\r': return "\\r";
			case '\n': return "\\n";
			case '\t': return "\\t";
			default: return stringf( "\\%03o", (int)c );
			}
		}
		else return string( 1, c ); // nothing to encode
	}

	FLUT_API char decode_char( const char* buf, size_t buf_size, int* len )
	{
		flut_error_if( buf_size == 0, "Invalid buffer size" );
		*len = 0;
		if ( *buf != '\\' || buf_size == 1 )
			return *buf;

		*len = 2;
		switch ( *(++buf) )
		{
		case 0: return '?'; // this is an error
		case '\\': return '\\';
		case '\"': return '\"';
		case 'r': return '\r';
		case 'n': return '\n';
		case 't': return '\t';
		default:
		{
			// read oct digits
			int value = 0;
			int max_len = std::min( (int)buf_size - 1, 3 );
			for ( *len = 1; *len <= max_len; ++(*len) )
			{
				if ( *buf >= '0' && *buf < '8' )
					value = value * 8 + *buf++ - '0';
				else break;
			}
			if ( *len > 1 && value > 0 && value < 32 )
				return char( value ); // we read a valid char
			else return '?'; // no valid char could be read
		}
		}
	}

	FLUT_API string quoted( const string& s )
	{
		string sout = "\"";
		for ( const char& c : s )
		{
			if ( c == '\"' || c == '\\' || c < 32 )
				sout += encode_char( c );
			else sout += c;
		}
		sout += "\"";
		return sout;
	}

	FLUT_API bool needs_quotes( const string& s )
	{
		for ( const char& c : s )
			if ( c < 33 || c == '\"' || c == '\\' )
				return true;
		return false;
	}

	FLUT_API string try_quoted( const string& s )
	{
		if ( needs_quotes( s ) )
			return quoted( s );
		else return s;
	}

	FLUT_API string try_unquoted( const string& s )
	{
		if ( s.empty() || s.front() != '\"' || s.back() != '\"' )
			return s; // no quotes

		string sout;
		for ( auto it = s.begin() + 1; *it != '\"'; ++it )
		{
			if ( *it == '\\' )
			{
				int len;
				sout += decode_char( &( *it ), s.end() - it, &len );
				it += ( len - 1 );
			}
			else sout += *it;
		}
		return sout;
	}

	flut::string get_filename_ext( const string& str )
	{
		size_t n = str.find_last_of( '.' );
		if ( n != string::npos ) {
			string ext = str.substr( n + 1 );
			if ( ext.find_last_of( "/\\" ) == string::npos ) // check if not part of folder
				return ext;
		}
		return string(); // no extension found
	}

	flut::string get_filename_without_ext( const string& str )
	{
		auto ext_len = get_filename_ext( str ).size();
		if ( ext_len > 0 ) ++ext_len; // add dot
		return str.substr( 0, str.size() - ext_len );
	}

	flut::string get_filename_folder( const string& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if ( n != string::npos ) return str.substr( 0, n + 1 );
		else return str;
	}

	flut::string get_filename_without_folder( const string& str )
	{
		size_t n = str.find_last_of( "/\\" );
		if ( n != string::npos ) return str.substr( n + 1, string::npos );
		else return str;
	}

}
