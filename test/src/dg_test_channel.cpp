// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_test_channel.cpp -- unit tests for dgd::channel
// 

extern "C" {
#include <dir.h>
};

#include <string>

#include <boost/mpl/list.hpp>

#include <boost/variant/recursive_variant.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <boost/filesystem.hpp>

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "dg_test_common.h"

#include <dg_exception.h>
#include <dg_wrapper_filter.h>
#include <dg_journal_filter.h>
#include <dg_flushable_sink.h>
#include <dg_channel.h>
#include <dg_manip.h>

#include "dg_parser.h"
#include "dg_parser_verifier.h"
#include "dg_temp_file.h"
#include "dg_file_verifier.h"

BOOST_TEST_CASE_TEMPLATE_FUNCTION( simple_channel, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;


   literal<char_type> test_string("Test string. Test 2 string 2.");
   temp_file log("channel_test", "log");
   std::string journal_path = "dummy.dummy";

   BOOST_TEST_MESSAGE("Temp log path: " << log.path().string());
   
   {
      BOOST_TEST_MESSAGE("Simple output to channel");
      
      {
         dgd::channel<char_type> ch;
         ch.open(log.path().string());

         BOOST_CHECK( ch.wrapper() != NULL );

         unsigned int max_width = 12;

         ch.wrapper()->max_width(max_width);

         ch << test_string.target << std::flush;

         BOOST_CHECK( ch.journal() != NULL );

         journal_path = ch.journal()->journal_file();

         BOOST_CHECK( fs::exists(journal_path) );
         BOOST_CHECK_GT( fs::file_size(journal_path), 0u );

         ch.force_sync();

         indented_line_glob glob = 
            make_glob(arg_indent <= 1u && arg_indent + strlen_(arg_text) <= val(max_width)) * 3;

         sane(log, ch.wrapper()->indent_step(), glob);
      }

      BOOST_CHECK( !fs::exists(journal_path) );

   }
}


BOOST_TEST_CASE_TEMPLATE_FUNCTION( channel_manip, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   temp_file log("channel_test", "log");
   std::string journal_path = "dummy.dummy";
   
   {
      BOOST_TEST_MESSAGE("Output to channel with manipulators");
      
      {
         dgd::channel<char_type> ch;
         ch.open(log.path().string());

         unsigned int max_width_1 = 12;
         unsigned int max_width_2 = 2;
         unsigned int indent_step = ch.wrapper()->indent_step();

         BOOST_CHECK( ch.wrapper() != NULL );
                
         ch << dgd::maxwidth(max_width_1) << test_string.target << std::endl

            << dgd::wrap(false) 
            << test_string.target << std::endl 
            << dgd::wrap(true) 

            << dgd::incr 
            << test_string.target 
            << dgd::decr
            << std::endl

            << dgd::maxwidth(max_width_2) 
            << test_string.target << std::endl;

         BOOST_CHECK( ch.journal() != NULL );

         journal_path = ch.journal()->journal_file();

         ch.force_sync();

         indented_line_glob glob = 
            make_glob(arg_indent <= 1u && arg_indent + strlen_(arg_text) <= val(max_width_1)) * 3 +
            make_glob(arg_indent == 0 && strlen_(arg_text) == val(test_string.target.length())) +
            make_glob(arg_indent >= val(indent_step) &&  arg_indent + strlen_(arg_text) <= val(max_width_1)) * 8 +
            make_glob(arg_indent <= 1u &&  arg_indent + strlen_(arg_text) <= val(max_width_2)) * 15;

         sane(log, indent_step, glob);
      }

      BOOST_CHECK( !fs::exists(journal_path) );
   }
}


class example_class {
   public:
      example_class(int a, std::string b, bool c):
         m_field_a(a),
         m_field_b(b),
         m_field_c(c) {
      }

      int field_a() const { return m_field_a; }
      std::string field_b() const { return m_field_b; }
      bool field_c() const { return m_field_c; }

   private:
      int m_field_a;
      std::string m_field_b;
      bool m_field_c;
};

template <typename char_type>
std::basic_ostream<char_type>&
operator <<(std::basic_ostream<char_type>& ostr, const example_class& inst)
{
   typename dgd::wrapper_filter<char_type>::wrapper_config  state;

   ostr << literal<char_type>("{")
        << dgd::save<char_type>(state)
        << std::endl
        << dgd::incr
        << literal<char_type>("field_a: ") << inst.field_a() << std::endl
        << literal<char_type>("field_b: ") 
        << literal<char_type>(inst.field_b()) << std::endl
        << literal<char_type>("field_c: ") << inst.field_c() << std::endl
        << dgd::restore<char_type>(state)
        << literal<char_type>("}");
   return ostr;
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( struct_example, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   temp_file log("example_class", "log");
   std::string journal_path = "dummy.dummy";
   
   {
      BOOST_TEST_MESSAGE("Complete channel example with class example_class");
      
      {
         dgd::channel<char_type> ch;
         ch.open(log.path().string());

         BOOST_CHECK( ch.wrapper() != NULL );

         unsigned int indent_step = ch.wrapper()->indent_step();
         unsigned int max_width = ch.wrapper()->max_width();
      
         example_class a(10, "Ten", true);
         example_class b(11, "Eleven", false);
         example_class c(12, "Tvelve", true);
         example_class d(99, "Nighty-nine", true);

         ch << dgd::incr
            << a << std::endl
            << literal<char_type>("-------------") << std::endl
            << b << std::endl
            << literal<char_type>("-------------") << std::endl
            << c << std::endl
            << literal<char_type>("-------------") << std::endl
            << d << std::endl
            << literal<char_type>("-------------") << std::endl;

         BOOST_CHECK( ch.journal() != NULL );

         journal_path = ch.journal()->journal_file();

         BOOST_CHECK( fs::exists(journal_path) );
         BOOST_CHECK_GT( fs::file_size(journal_path), 0u );

         ch.force_sync();

         indented_line_glob glob = 
            (make_glob
             (arg_scope == 0 && arg_indent == val(indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
             (arg_scope == 1 && arg_indent == val(2*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
             (arg_scope == 1 && arg_indent == val(2*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
             (arg_scope == 1 && arg_indent == val(2*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
             (arg_scope == 0 && arg_indent == val(indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
             (arg_scope == 0 && arg_indent == val(indent_step) && arg_indent + strlen_(arg_text) <= val(max_width))
            ) * 4;

         sane(log, indent_step, glob);
      }

      BOOST_CHECK( !fs::exists(journal_path) );
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( bad_log_name, char_type )
{
   namespace fs = boost::filesystem;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   std::string log_path ;
   bool dirty;
   {
      BOOST_TEST_MESSAGE("Opening channel with empty log");
      {
         log_path = "";
         dirty = false;
         try {
            dgd::channel<char_type> ch;
            ch.open(log_path);
            
            ch << test_string.target << std::flush;
         } catch( dgd::exception &ex ) {
            BOOST_TEST_MESSAGE("caught: " << ex.what());
            dirty = true;
         }
         
         BOOST_CHECK( dirty );
      }

      BOOST_TEST_MESSAGE("Opening channel with dir name");
      {         
         log_path = "./.";
         dirty = false;
         try {
            dgd::channel<char_type> ch;
            ch.open(log_path);
            
            ch << test_string.target << std::flush;
         } catch( dgd::exception &ex ) {
            BOOST_TEST_MESSAGE("caught: " << ex.what());
            dirty = true;
         }

         BOOST_CHECK(dirty);
      }

      BOOST_TEST_MESSAGE("Opening channel with read-only file");
      {         
         std::string journal_path;
         log_path = "channel-permission-test.log";
         std::ofstream log(log_path.c_str());        
         log << "dummy" << std::flush;
         log.close();

         chmod(log_path.c_str(), 0444);

         dirty = false;
         try {
            dgd::channel<char_type> ch;
            ch.open(log_path);
            
            ch << test_string.target << std::flush;

            journal_path = ch.journal()->journal_file();

            BOOST_CHECK( fs::exists(journal_path) );
            BOOST_CHECK_GT( fs::file_size(journal_path), 0u );            
         } catch( std::exception &ex ) {
            BOOST_TEST_MESSAGE("caught: " << ex.what());
            dirty = true;
         }

         BOOST_CHECK(dirty);

         chmod(log_path.c_str(), 0777);
         fs::remove(log_path);

         if(fs::exists(journal_path)) {
            fs::remove(journal_path);
         }
      }

   }
}

bool init_test()
{
   typedef boost::mpl::list<char, wchar_t> char_types;

   ::boost::unit_test::framework::master_test_suite().
       add( BOOST_TEST_CASE_TEMPLATE( simple_channel, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
       add( BOOST_TEST_CASE_TEMPLATE( channel_manip, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
       add( BOOST_TEST_CASE_TEMPLATE( struct_example, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
       add( BOOST_TEST_CASE_TEMPLATE( bad_log_name, char_types ) );
 
   return true;
}

int main( int argc, char* argv[] )
{
    return ::boost::unit_test::unit_test_main( &init_test, argc, argv );
}

// 
// Local Variables:
// compile-command: "make dg_test_channel.obj"
// End:
//


