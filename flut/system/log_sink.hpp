#pragma once

#include "log.hpp"
#include <fstream>

namespace flut
{
	namespace log
	{
		class FLUT_API sink
		{
		public:
			sink( level l = info_level ) : log_level_( l ) { flut::log::add_sink( this ); }
			virtual ~sink() { flut::log::remove_sink( this ); }
			virtual void send_log_message( level l, const string& msg ) = 0;
			void try_send_log_message( level l, const string& msg ) { if ( test_log_level( l ) ) send_log_message( l, msg ); }
			void set_log_level( level l ) { log_level_ = l; flut::log::get_global_log_level(); }
			level get_log_level() { return log_level_; }
			bool test_log_level( level l ) { return l >= log_level_; }

		protected:
			level log_level_;
		};

		class stream_sink : public sink
		{
		public:
			stream_sink( level l, std::ostream& str = std::cout ) : sink( l ), stream_( str ) {}
			virtual void send_log_message( level l, const string& msg ) { stream_ << msg << std::endl; };

		protected:
			std::ostream& stream_;
		};

		class file_sink : public stream_sink
		{
		public:
			file_sink( level l, const path& file ) : file_stream_( file.str() ), stream_sink( l, file_stream_ ) {}
			virtual void send_log_message( level l, const string& msg ) override {
				if ( file_stream_.good() ) { stream_sink::send_log_message( l, msg ); file_stream_.flush(); }
			}
			bool good() { return file_stream_.good(); }

		protected:
			std::ofstream file_stream_;
		};
	}
}
