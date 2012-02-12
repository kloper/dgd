// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_flushable_sink.h -- implement flushable boost::iostreams::file_sink
// 

#ifndef _dg_flushable_sink_h_
#define _dg_flushable_sink_h_

namespace dgd
{

template<typename Ch>
class basic_flushable_sink {
   public:
      typedef Ch char_type;
      struct category
         : public boost::iostreams::output,
           public boost::iostreams::device_tag,
           public boost::iostreams::flushable_tag,
           public boost::iostreams::closable_tag,
           public boost::iostreams::localizable_tag
      { };

      basic_flushable_sink( const std::string& path,
                            std::ios_base::openmode mode = std::ios_base::out ) 
      {         
         m_file_ptr.reset( new std::basic_ofstream<char_type>( path.c_str(), 
                                                               mode ) );
      }
      
      std::streamsize write(const char_type* s, std::streamsize n) {
         m_file_ptr->write(s, n);
         return n;
      }

      bool is_open() const { return m_file_ptr->is_open(); }
      void close() { m_file_ptr->close(); } 
      bool flush() { m_file_ptr->flush(); return true; }
      void imbue(const std::locale& loc) { m_file_ptr->imbue(loc);  }

   private:
      boost::shared_ptr< std::basic_ofstream<char_type> > m_file_ptr;
};


}; // end of namespace dgd

#endif /* _dg_flushable_sink_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_flushable_sink.obj"
 * End:
 */


