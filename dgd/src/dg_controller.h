// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_controller.h -- per-thread singleton that keeps channel and 
//                    filters-out logging requests
// 

#ifndef _dg_controller_h_
#define _dg_controller_h_

namespace dgd {

template <typename Ch, typename FTraits = filter_traits>
class controller {
   public:      
      typedef controller<Ch> self_type;
      typedef channel<Ch> channel_type;
      typedef typename channel_type::char_type char_type;
      typedef typename channel_type::wrapper_config wrapper_config;
      typedef boost::thread_specific_ptr<self_type> thread_local_ptr_type;
      typedef typename channel_type::string_type string_type;
      typedef FTraits filter_traits;
      typedef typename filter_traits::filter_type filter_type;
      typedef typename filter_traits::value_type  filter_value_type;

   public:
      static void init(int argc, char **argv) {
         namespace po = boost::program_options;
         po::options_description desc("DGD Tracing Options");
         desc.add_options()
            ("trace-help", "show tracing command line help")
            ("trace-journal-size", po::value<unsigned int>(), 
             "trace journal size")
            ("trace-log", po::value<std::string>(), "trace log file name")
            ("trace-indent-step", po::value<unsigned int>(), "indentation step")
            ("trace-min-width", po::value<unsigned int>(), 
             "minium width of pre-indented line in the log")
            ("trace-max-width", po::value<unsigned int>(), 
             "maximum width of post-indented line in the log")
            ("trace-wrap", po::value<bool>(), 
             "enable/disable line wrapping in log")
            ("trace-word-wrap", po::value<bool>(), 
             "enable/disable word wrapping in log")
            ("trace-spaces", po::value<std::string>(), 
             "affects which characters are considered as spaces between words")
            ("trace-filter", po::value<std::string>(), 
             "define which trace message will be really logged");

         po::variables_map vm;
         po::store(po::parse_command_line(argc, argv, desc), vm);
         po::notify(vm);    

         if(vm.count("trace-help") > 0) {
            std::ostringstream ostr;
            ostr << desc << std::endl;
            throw dgd::exception(ostr.str());
         }

         if(vm.count("trace-log") > 0) {
            m_log_file_name = vm["trace-log"].as<std::string>();
            m_enabled = true;
         }

         if(vm.count("trace-journal-size") > 0) {
            m_journal_size = vm["trace-journal-size"].as<unsigned int>();
         } else {
            m_journal_size = journal_filter<char_type>::DefaultJournalSize;
         }

         if(vm.count("trace-indent-step") > 0) {
            m_wrapper_config.indent_step(
               vm["trace-indent-step"].as<unsigned int>()
            );
         } 

         if(vm.count("trace-min-width") > 0) {
            m_wrapper_config.min_width(
               vm["trace-min-width"].as<unsigned int>()
            );
         } 

         if(vm.count("trace-max-width") > 0) {
            m_wrapper_config.max_width(
               vm["trace-max-width"].as<unsigned int>()
            );
         } 

         if(vm.count("trace-wrap") > 0) {
            m_wrapper_config.wrap(
               vm["trace-wrap"].as<bool>()
            );
         } 

         if(vm.count("trace-word-wrap") > 0) {
            m_wrapper_config.word_wrap(
               vm["trace-word-wrap"].as<bool>()
            );
         } 

         if(vm.count("trace-strings") > 0) {
            m_wrapper_config.spaces(
               literal<char_type>(
                  vm["trace-strings"].as<std::string>().c_str()
               ).value
            );
         } 

         if(vm.count("trace-filter") > 0) {
            m_filter = filter_traits::parse(
               vm["trace-filter"].as<std::string>()
            );
         } else {
            m_filter = filter_traits::allow_all();            
         }
      }

      static self_type *get() {
         namespace fs = boost::filesystem;

         if( m_controller.get() != NULL ) {
            return m_controller.get();
         }

         if( !m_enabled ) {
            return NULL;
         }

         self_type *local_controller = NULL;
         
         try {
            std::ostringstream ostr;
            fs::path log_file_path(m_log_file_name);
            std::string extension = log_file_path.extension().string();
            
            ostr << "." << boost::this_thread::get_id() << extension
                 << std::flush;
         
            log_file_path.replace_extension(ostr.str());

            local_controller = new self_type();
            local_controller->get_channel().open( 
               log_file_path.string(),
               m_journal_size > 0 ? channel_type::mode::journal : 0,
               m_journal_size,
               m_wrapper_config
            );            
         } catch( std::exception &ex ) {
            local_controller->last_error(ex.what());
            local_controller->enabled(false);
         }

         m_controller.reset(local_controller);

         m_controller->enabled(true);
         return m_controller.get();
      }

      static void reset() {
         m_controller.reset(NULL);
      }

   public:
      channel_type &get_channel() {
         return m_channel;
      }

      bool enabled() const {
         return m_local_enabled;
      }

      std::string last_error() const {
         return m_last_error;
      }

      bool filter(const filter_value_type& val) {
         return (m_local_enabled && filter_traits::match(m_filter, val));
      }

   protected:
      controller() :
         m_local_enabled(false)
      {
      }

      void last_error(const std::string &val) {
         m_last_error = val;
      }

      void enabled(bool val) {
         m_local_enabled = val;
      }

   private:
      static bool m_enabled;
      static std::string m_log_file_name;
      static wrapper_config m_wrapper_config;
      static unsigned int m_journal_size;
      static thread_local_ptr_type m_controller;
      static filter_type m_filter;

   private:
      channel_type m_channel;
      bool m_local_enabled;
      std::string m_last_error;      
};

template <typename Ch, typename Tr>
bool controller<Ch, Tr>::m_enabled = false;

template <typename Ch, typename Tr>
std::string controller<Ch, Tr>::m_log_file_name;

template <typename Ch, typename Tr>
typename controller<Ch, Tr>::wrapper_config controller<Ch, Tr>::m_wrapper_config;

template <typename Ch, typename Tr>
unsigned int controller<Ch, Tr>::m_journal_size;

template <typename Ch, typename Tr>
typename controller<Ch, Tr>::thread_local_ptr_type 
controller<Ch, Tr>::m_controller;

template <typename Ch, typename Tr>
typename controller<Ch, Tr>::filter_type controller<Ch, Tr>::m_filter;

}; // end of namespace dgd


#endif /* _dg_controller_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_controller.obj"
 * End:
 */


