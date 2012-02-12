// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_wrapper_filter.h -- boost::iostreams::filter that indents and 
//                        wraps lines of input
// 

#ifndef _dg_wrapper_filter_h_
#define _dg_wrapper_filter_h_

namespace dgd
{

template<typename Ch>
class literal {
   public:
      typedef std::basic_string<Ch> value_type;
      value_type value;

      literal(const char* val) : value(val) {}        
};

template<>
class literal<wchar_t> {
   public:
      typedef std::basic_string<wchar_t> value_type;
      value_type value;

      literal(const char* val) {
         while(val != NULL && *val > 0) {
            value += std::btowc(*val);
            val++;
         }
      }        
};

template<typename Ch>
class wrapper_filter: 
      public boost::iostreams::multichar_filter<boost::iostreams::output, Ch> 
{
   public:
      typedef 
      boost::iostreams::multichar_filter<boost::iostreams::output, Ch> Parent;

      typedef typename Parent::char_type char_type;

      enum DefaultValues { 
	 DefaultBufferSize = 100,
	 DefaultMaxWidth   = 79,
	 DefaultMinWidth   = 20,
	 DefaultIndentStep = 8 
      };

      class wrapper_config {
         public:
            typedef std::basic_string<char_type> string_type;

         public:
            wrapper_config() :
               m_indent(0),
               m_indent_step(DefaultIndentStep),
               m_min_width(DefaultMinWidth),
               m_max_width(DefaultMaxWidth),
               m_wrap(true),
               m_word_wrap(true),
               m_spaces(literal<char_type>(" \t").value) {}

            wrapper_config( unsigned int indent,
                            unsigned int indent_step,
                            unsigned int min_width,
                            unsigned int max_width,
                            bool wrap,
                            bool word_wrap,
                            const string_type& spaces) :
               m_indent(indent),
               m_indent_step(indent_step),
               m_min_width(min_width),
               m_max_width(max_width),
               m_wrap(wrap),
               m_word_wrap(word_wrap),
               m_spaces(spaces) {}
               
            unsigned int indent() const { return m_indent; }
            unsigned int indent_step() const { return m_indent_step; }
            unsigned int min_width() const { return m_min_width; }
            unsigned int max_width() const { return m_max_width; }
            bool wrap() const { return m_wrap; }
            bool word_wrap() const { return m_word_wrap; }
            string_type spaces() const { return m_spaces; }

            void indent(unsigned int val) { m_indent = val; }
            void indent_step(unsigned int val) { m_indent_step = val; }
            void min_width(unsigned int val) { m_min_width = val; }
            void max_width(unsigned int val) { m_max_width = val; }
            void wrap(bool val) { m_wrap = val; }
            void word_wrap(bool val) { m_word_wrap = val; }
            void spaces(const string_type& val) { m_spaces = val; }

         private:
            unsigned int m_indent;
            unsigned int m_indent_step;
            unsigned int m_min_width;
            unsigned int m_max_width;
            bool m_wrap;
            bool m_word_wrap;
            string_type m_spaces;
      };

      typedef typename wrapper_config::string_type string_type;

   public:
      wrapper_filter(const wrapper_config& config = wrapper_config() ) :
         Parent(),
         m_line(0),
         m_column(0),
         m_bytes(0),
         m_config(config),
         m_remaining_eol_chars(0),
         m_indent_pending(false) {
         std::basic_ostringstream<char_type> endl_stream;
         endl_stream << std::endl << std::flush;
         m_eol = endl_stream.str();
         m_indent_str.resize(m_config.indent_step(), ' ');
      }

      wrapper_filter(const wrapper_filter &peer) :
         Parent(peer),
         m_line(peer.m_line),
         m_column(peer.m_column),
         m_bytes(peer.m_bytes),
         m_config(peer.m_config),
         m_eol(peer.m_eol),
         m_remaining_eol_chars(peer.m_remaining_eol_chars),
         m_indent_str(peer.m_indent_str),
         m_indent_pending(peer.m_indent_pending) {
      }

   public:
      template<typename Sink>
      std::streamsize write(Sink& dest, const char_type* s, std::streamsize n)
      {
         if(m_remaining_eol_chars > 0) {
            std::streamsize rc = end_write_eol(dest);
            if( m_remaining_eol_chars > 0 ) 
               return rc;
         }

         const char_type* current = s;
         const char_type* endpos = s + n;         
         std::streamsize chars_written = 0;

         while(current < endpos) 
         {
            if( (m_column == 0 || m_indent_pending > 0) && 
                write_indent(dest) > 0 ) {
               return 0;
            }

            const char_type* breakpos = NULL;         
            bool require_eol = false;

            const char_type* eolpos = 
               std::find_first_of( current, endpos, 
                                   m_eol.begin(), m_eol.end() );

            breakpos = find_word_break(current, eolpos);
            if(breakpos == NULL)
            {
               breakpos = eolpos; 
               if( eolpos < endpos ) {
                  require_eol = true;
               }
            } else {
               require_eol = true;
            }

            std::streamsize length = breakpos-current;
            std::streamsize rc = boost::iostreams::write(dest, current, length);
       
            chars_written += rc;
            m_column += rc;
            m_bytes += rc;

            if(require_eol && rc == length)
            {
               rc = begin_write_eol(dest);
               if( m_remaining_eol_chars > 0 ) {
                  return chars_written;
               }
            } else {
               return chars_written;
            }

            while(breakpos < endpos && 
                  m_eol.find(*breakpos) != 
                  std::basic_string<char_type>::npos ) {
               breakpos++;
               chars_written++;
            }
            
            current = breakpos;
         }

         return chars_written;
      }

      template<typename Sink>
      void close(Sink& snk)
      {
         m_remaining_eol_chars = 0;
         m_indent_pending = 0;         
         m_column = 0;
         m_line = 0;
         m_bytes = 0;
         m_config.indent(0);
      }

   private:

      template<typename Sink>
      unsigned int  write_indent(Sink &dest) {
         if(m_indent_pending > 0) {
            std::basic_string<char_type> spaces;
            spaces.resize( m_indent_pending, ' ' );

            std::streamsize rc = 
               boost::iostreams::write(dest, spaces.c_str(), m_indent_pending);
            m_bytes += rc;
            m_column += rc;

            m_indent_pending -= rc;            
            if( m_indent_pending > 0 ) {
               return m_indent_pending;
            }
         } 

         int indent_size = 
            std::min( m_config.indent(), 
                      (m_config.max_width() - 
                       m_config.min_width() - m_column) / 
                      m_config.indent_step());
         
         std::basic_string<char_type> spaces;
         spaces.resize(  m_config.indent_step(), ' ' );
         
         while(m_indent_pending == 0 && indent_size-- > 0) {
            std::streamsize rc = 
               boost::iostreams::write(dest, spaces.c_str(), 
                                       m_config.indent_step());
            m_bytes += rc;
            m_column += rc;
            
            m_indent_pending = m_config.indent_step() - rc;
         }

         return m_indent_pending;
      }

      const char_type* find_word_break(const char_type* current, 
                                       const char_type* endpos)
      {
         const char_type* breakpos = NULL;

         if(m_config.wrap()) {
            if(m_config.word_wrap()) {
               bool spacepos_moved = false;
               const char_type* spacepos = current;
               const char_type* pos = NULL;

               string_type spaces = m_config.spaces();
               unsigned int max_width = m_config.max_width() - m_column;

               do {
                  while(spacepos < endpos && 
                        (spaces.find(*spacepos) != string_type::npos)) {
                     if( (unsigned int)(spacepos-current) < max_width )
                        spacepos++;
                     else {
                        return spacepos;
                     }
                  }

                  pos = std::find_first_of( spacepos, endpos, 
                                            spaces.begin(), 
                                            spaces.end());

                  if( (unsigned int)(pos-current) <= max_width) {
                     spacepos = pos;
                     spacepos_moved = true;
                  } else {
                     if(spacepos_moved) {
                        breakpos = spacepos;
                     }
                     break;
                  }       
               } while(pos < endpos);
            } 

            if(breakpos == NULL) {
               const char_type* widthpos = 
                  current + m_config.max_width() - m_column;

               if( widthpos < endpos ) {
                  breakpos = widthpos;
               }
            }
         }

         return breakpos;
      }

      template<typename Sink>
      std::streamsize begin_write_eol(Sink& dest) {
         m_remaining_eol_chars = m_eol.length();
         return end_write_eol(dest);
      }

      template<typename Sink>
      std::streamsize end_write_eol(Sink& dest) {
         std::streamsize rc = 
            boost::iostreams::write(dest, 
                          m_eol.c_str() + m_eol.length() - 
                                           m_remaining_eol_chars, 
                          m_remaining_eol_chars);
         m_remaining_eol_chars -= rc;         
         m_bytes += rc;

         if(m_remaining_eol_chars == 0) {
            m_column = 0;
            m_line++;
         }

         return rc;
      }


   public:
      unsigned int line()   const { return m_line; }
      unsigned int column() const { return m_column; }

      void         indent_step( unsigned int step ) { 
         m_config.indent_step(step); 
         m_indent_str.resize(step, ' ');
      }

      unsigned int indent_step() const { 
         return m_config.indent_step(); 
      }

      void         incr_indent() { 
         m_config.indent( m_config.indent() + 1);
      }

      void         decr_indent() { 
         m_config.indent( m_config.indent() - 1);
      }

      void         indent(const unsigned int indent) {
         m_config.indent(indent);
      }

      unsigned int indent() const { 
         return m_config.indent(); 
      }
      
      void         min_width( unsigned int width ) { 
         m_config.min_width(width); 
      }

      unsigned int min_width() const { 
         return m_config.min_width(); 
      }

      void         max_width( unsigned int width ) { 
         m_config.max_width(width); 
      }

      unsigned int max_width() const { 
         return m_config.max_width(); 
      }
      
      void         wrap( bool allow ) { 
         m_config.wrap(allow);
      }

      bool         wrap() const { 
         return m_config.wrap(); 
      }

      void         word_wrap( bool allow ) {
         m_config.word_wrap(allow); 
      }

      bool         word_wrap() const { 
         return m_config.word_wrap(); 
      }
      
      void         space_chars(const string_type& spc) { 
         m_config.spaces(spc); 
      }

      string_type space_chars() const { 
         return m_config.spaces();
      }

      unsigned long bytes_written() const { return m_bytes; }

   private:
      unsigned int m_line;
      unsigned int m_column;
      unsigned long m_bytes;

      wrapper_config m_config;

      std::basic_string<char_type> m_eol;
      int m_remaining_eol_chars;
      std::basic_string<char_type> m_indent_str;
      unsigned int m_indent_pending;
};

}; // end of namespace dgd

#endif /* _dg_wrapper_filter_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_wrapper_filter.obj"
 * End:
 */


