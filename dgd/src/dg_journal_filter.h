// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_journal_filter.h -- transparent boost::iostreams::filter that keeps 
//                        memory mapped circular buffer for unexpected crash 
//                        recovering
// 

#ifndef _dg_journal_filter_h_
#define _dg_journal_filter_h_

namespace dgd
{

template<typename Ch>
class journal_filter: 
      public boost::iostreams::multichar_filter<boost::iostreams::output, Ch> 
{      
   public:
      typedef 
      boost::iostreams::multichar_filter<boost::iostreams::output, Ch> Parent;

      typedef typename Parent::char_type char_type;
      
      const std::string journal_name_suffix;
      
      enum DefaultValues {
         DefaultJournalSize = sizeof(char_type) * 10 * 1024
      };

   private:
      struct circular_buffer {
            unsigned int char_size;
            unsigned int size;
            unsigned int head;
            unsigned int tail;

            std::streamsize chars_size() const {
               return (size - sizeof(circular_buffer)) / char_size;
            }
            
            char_type* buffer() {    
               return (char_type*)((char*)this + sizeof(circular_buffer));
            }
      };



   public:
      journal_filter(std::string journal_name, 
                     std::streamsize journal_size = DefaultJournalSize) : 
         Parent(),
         journal_name_suffix(".journal"),
         m_journal_size(journal_size),
         m_journal(NULL),
         m_buffer(NULL)
      {
         m_journal_name = full_journal_name(journal_name);
         create_journal_file();
         map_journal();
      }

      template<typename Sink>
      std::streamsize write(Sink& dest, const char_type* s, std::streamsize n)
      {
         circular_buffer *buffer = (circular_buffer*)m_buffer->get_address();

         const char_type* current = s;
         const char_type* endpos = s + n;         
         std::streamsize chars_written = 0;

         while(current < endpos)
         {
            unsigned int buffer_start = 0;
            unsigned int buffer_end = buffer->chars_size();
            const char_type* startpos = current;

            if( buffer->head <= buffer->tail ) {
               current += 
                  fill(buffer, current, endpos, buffer->tail, buffer_end); 
               current += 
                  fill(buffer, current, endpos, buffer_start, buffer->head);
            } else if( buffer->head > buffer->tail ) {
               current += 
                  fill(buffer, current, endpos, buffer->tail, buffer->head); 
            } 

            std::streamsize length = current - startpos;
            std::streamsize rc = 
               boost::iostreams::write( dest, startpos, length );
            
            chars_written += rc;
            buffer->head = (buffer->head + rc) % buffer->chars_size();
            if(rc < length)
               return chars_written;                       
         }

         return chars_written;
      }

      template<typename Sink>
      void close(Sink& dest)
      {
         close();
      }

      void close() {
         if(m_buffer != NULL) {
            m_buffer->flush();
            
            circular_buffer *buffer = (circular_buffer*)m_buffer->get_address();
            bool journal_delete = (buffer->head == buffer->tail);
            
            delete m_buffer;
            delete m_journal;

            if( journal_delete ) {
               boost::filesystem::remove( m_journal_name);
            }            
         }
      }

      std::string journal_file() const { return m_journal_name; }

   private:
      std::string full_journal_name(std::string journal_name) const {         
         std::string::size_type suffix_index = 
            journal_name.rfind(journal_name_suffix);
         if( suffix_index == 
             (journal_name.length() - journal_name_suffix.length()) ) 
         {
            journal_name = journal_name.substr( 0, suffix_index+1 );
         }

         std::ostringstream ostr;
         ostr << journal_name << journal_name_suffix;
            
         return ostr.str();
      }

      std::streamsize fill( circular_buffer *buffer,
                            const char_type* current, 
                            const char_type* endpos,
                            unsigned int start, 
                            unsigned int end ) {
         char_type *buffer_start = buffer->buffer();

         std::streamsize chars_to_copy = 
            std::min( (unsigned int)(endpos-current), end-start);
         if(chars_to_copy > 0) {
            std::copy( current, current + chars_to_copy, buffer_start + start );
            buffer->tail = (start + chars_to_copy) % buffer->chars_size();
         }
         
         return chars_to_copy;
      }

      void create_journal_file() {
         std::streamsize mapped_buffer_size = 
            sizeof(circular_buffer) + m_journal_size;


         std::ofstream journal_file(m_journal_name.c_str(), 
                                    std::ios_base::out |
                                    std::ios_base::trunc |
                                    std::ios_base::binary );
         for(int i = 0; i < mapped_buffer_size; i++)
         {
            journal_file.put(0);
         }
         journal_file.close();
      }

      void map_journal() {
         namespace ipc = boost::interprocess;

         std::streamsize mapped_buffer_size = 
            sizeof(circular_buffer) + 
            m_journal_size / sizeof(char_type) * sizeof(char_type);

         m_journal = new ipc::file_mapping( m_journal_name.c_str(), 
                                            ipc::read_write);
         m_buffer = new ipc::mapped_region( *m_journal, 
                                            ipc::read_write,
                                            0,
                                            mapped_buffer_size );

         char *buffer = (char*)m_buffer->get_address();
         std::fill( buffer, buffer+mapped_buffer_size, 0);

         circular_buffer *cbuffer = (circular_buffer*)buffer;
         cbuffer->char_size = sizeof(char_type);
         cbuffer->size = mapped_buffer_size;
         cbuffer->head = 0;
         cbuffer->tail = 0;         
      }


   private:
      std::string m_journal_name;
      std::streamsize m_journal_size;

      boost::interprocess::file_mapping *m_journal;
      boost::interprocess::mapped_region *m_buffer;
};

} // end of namespace dgd

#endif /* _dg_journal_filter_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_journal_filter.obj"
 * End:
 */


