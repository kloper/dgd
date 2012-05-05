// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_channel.h -- boost::filtering_stream controlling dgd filters
// 

#ifndef _dg_channel_h_
#define _dg_channel_h_

namespace dgd {

template<typename Ch>
class channel: 
         public boost::iostreams::filtering_stream<boost::iostreams::output, Ch>
{
   public:
      typedef 
      boost::iostreams::filtering_stream<boost::iostreams::output, Ch> Parent;
      typedef typename Parent::char_type char_type;

      typedef journal_filter<char_type> journal_filter_type;
      typedef wrapper_filter<char_type> wrapper_filter_type;
      typedef fake_flushable_sink<char_type> sink_type;
      typedef typename wrapper_filter_type::wrapper_config wrapper_config;
      typedef typename wrapper_config::string_type string_type;

      struct mode {            
            const static unsigned int journal = 0x00000001;
      };

   public:
      channel() :
         Parent(),
         m_journal_filter(NULL),
         m_wrapper_filter(NULL),
         m_is_open(false) {
      }

      ~channel() {
         if(m_is_open) force_sync();
      }

      void open( const std::string& log_file_name, 
                 unsigned int flags = mode::journal,
                 std::streamsize journal_size = 
                                       journal_filter_type::DefaultJournalSize,
                 const wrapper_config& config = wrapper_config() ) 
      {
         namespace fs = boost::filesystem;
         namespace io = boost::iostreams;

         if(log_file_name.empty()) {
            throw dgd::exception("empty log file name");
         }
         
         if(fs::is_directory(log_file_name)) {
            throw dgd::exception("log file cannot be directory");
         }

         sink_type logfile;
         logfile.open(log_file_name, std::ios_base::out |
                                        std::ios_base::ate);
         
         if(!logfile.is_open()) {
            throw dgd::exception("can't open log file for writing");
         }

         if((flags & mode::journal) != 0) {
            this->push(journal_filter_type(journal_name(log_file_name), 
                                           journal_size));
            m_journal_filter = this->component<journal_filter_type>(this->size()-1); 
         }

         this->push(wrapper_filter_type(config));
         m_wrapper_filter = this->component<wrapper_filter_type>(this->size()-1); 

         m_log_file_name = log_file_name;

         this->push(logfile);
         m_log_file = this->component<sink_type>(this->size()-1);

         m_is_open = true;
      }

      bool is_open() const { return m_is_open; }

      wrapper_filter_type* wrapper() const { return m_wrapper_filter; }
      journal_filter_type* journal() const { return m_journal_filter; }

      std::string log_file_name() const { return m_log_file_name; }

      void force_sync()
      {
         this->flush();
         m_log_file->sync();
      }

   private:
      std::string journal_name(std::string log_file_name) {
         namespace fs = boost::filesystem;

         fs::path log_file_path(log_file_name);

         std::string extension = log_file_path.extension().string();         
         if(!extension.empty()) {

            boost::to_upper(extension);
            if(extension == ".LOG" || extension == ".TXT") 
               return log_file_path.replace_extension().string();
         }

         return log_file_name;
      }

   private:
      journal_filter_type *m_journal_filter;
      wrapper_filter_type *m_wrapper_filter;
      sink_type           *m_log_file;
      std::string m_log_file_name;
      bool m_is_open;
};

} // end of namespace dgd

#endif /* _dg_channel_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_channel.obj"
 * End:
 */
