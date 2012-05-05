// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_flushable_sink.h -- implement fake flushable boost::iostreams::file_sink
// 

#ifndef _dg_flushable_sink_h_
#define _dg_flushable_sink_h_

namespace dgd
{

template<typename Ch>
class fake_flushable_sink {
   public:
      typedef Ch char_type;
      struct category
         : public boost::iostreams::output,
           public boost::iostreams::device_tag,
           public boost::iostreams::flushable_tag,
           public boost::iostreams::closable_tag,
           public boost::iostreams::localizable_tag
      { };

      fake_flushable_sink() 
      {         
      }

      void open( const std::string& path,
                 std::ios_base::openmode mode = std::ios_base::out ) 
      {         
         m_file_ptr.reset( new std::basic_ofstream<char_type>( path.c_str(), 
                                                               mode ) );
         
         int size = 1024*1024;
         char_type *buf = new char_type[size];
         
         m_file_ptr->rdbuf()->pubsetbuf(buf, size);
      }
      
      std::streamsize write(const char_type* s, std::streamsize n) {
         if(m_file_ptr.get() != NULL) {
            m_file_ptr->write(s, n);
            return n;
         }
         
         return -1;
      }

      bool is_open() const { 
         return (m_file_ptr.get() != NULL && 
                 m_file_ptr->is_open()); 
      }

      void close() { 
         if(m_file_ptr.get() != NULL) {
            m_file_ptr.reset();
         }
      }

      // This is a fake flush. The file is never flushed when requested by std::flush
      bool flush() { return true; }

      void imbue(const std::locale& loc) { 
         if(m_file_ptr.get() != NULL) 
            m_file_ptr->imbue(loc);  
      }

      // This is a real flush. It must be explicitly requested.
      void sync() { 
         if(m_file_ptr.get() != NULL) 
            m_file_ptr->flush(); 
      }

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


