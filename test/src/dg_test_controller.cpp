// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_test_controller.cpp -- test dgd::controller
// 

#include <string>

#include <boost/mpl/list.hpp>

#include <boost/variant/recursive_variant.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <boost/filesystem.hpp>

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>

#include <boost/program_options.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <boost/thread/tss.hpp>
#include <boost/thread/thread.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>

#include "dg_test_common.h"

#include <dg_exception.h>
#include <dg_wrapper_filter.h>
#include <dg_journal_filter.h>
#include <dg_flushable_sink.h>
#include <dg_channel.h>
#include <dg_manip.h>
#include <dg_controller.h>
#include <dg_scope.h>
#include <dg_expand.h>

#include "dg_parser.h"
#include "dg_parser_verifier.h"
#include "dg_temp_file.h"
#include "dg_file_verifier.h"

BOOST_TEST_CASE_TEMPLATE_FUNCTION( simple_controller, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   temp_file log("controller_test", "log");
   std::string journal_path = "dummy.dummy";
   
   {
      BOOST_TEST_MESSAGE("Test dgd::controller::get()");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char_type>::init(argc, argv);

         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK( ctrl != NULL );

         dgd::channel<char_type> &ch = ctrl->get_channel();

         BOOST_CHECK( ch.wrapper() != NULL );

         ch << test_string.target << std::flush;

         BOOST_CHECK( ch.journal() != NULL );

         journal_path = ch.journal()->journal_file();

         BOOST_CHECK( fs::exists(journal_path) );
         BOOST_CHECK_GT( fs::file_size(journal_path), 0 );

         unsigned int indent_step = ch.wrapper()->indent_step();
         temp_file real_log(ch.log_file_name());

         dgd::controller<char_type>::reset();

         indented_line_glob glob =
            make_glob(arg_indent == 0 && strlen_(arg_text) == val(test_string.target.length()));

         sane(real_log, indent_step, glob);
      }

      BOOST_CHECK( !fs::exists(journal_path) );
   }
}

template <typename char_type>
void controller_main(const literal<char_type> &test_string,
                     std::string *journal_path) {
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   std::string trace_log_arg = 
      std::string("--trace-log=") + log.path().string();

   int argc = 2;
   char *argv[3];
   argv[0] = (char*)"exe";
   argv[1] = (char*)trace_log_arg.c_str();
   argv[2] = NULL;

   dgd::controller<char_type>::init(argc, argv);
   
   dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();
   
   BOOST_CHECK( ctrl != NULL );

   dgd::channel<char_type> &ch = ctrl->get_channel();

   BOOST_CHECK( ch.wrapper() != NULL );   
   
   ch << test_string.target << std::flush;

   BOOST_CHECK( ch.journal() != NULL );
   
   *journal_path = ch.journal()->journal_file();

   BOOST_CHECK( fs::exists(*journal_path) );
   BOOST_CHECK_GT( fs::file_size(*journal_path), 0 );

   ch << test_string.target;
   
   unsigned int indent_step = ch.wrapper()->indent_step();
   temp_file real_log(ch.log_file_name());

   dgd::controller<char_type>::reset();

   indented_line_glob glob =
      make_glob(arg_indent == 0 && strlen_(arg_text) == val(2*test_string.target.length()));
   
   sane(real_log, indent_step, glob);
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( controller_in_thread, char_type )
{
   namespace fs = boost::filesystem;

   literal<char_type> test_string("Test string. Test 2 string 2.");   
   std::string journal_path = "dummy.dummy";
   
   {
      BOOST_TEST_MESSAGE("Test dgd::controller::get() in separate thread");
      
      {
         boost::thread runner(controller_main<char_type>, 
                              test_string, 
                              &journal_path);

         runner.join();
      }

      BOOST_CHECK( !fs::exists(journal_path) );
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( controller_help, char_type )
{
   namespace fs = boost::filesystem;
   
   {
      BOOST_TEST_MESSAGE("Test dgd::controller::init(--trace-help)");
      
      {
         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)"--trace-help";
         argv[2] = NULL;

         bool dirty = false;
         try {
            dgd::controller<char_type>::init(argc, argv);
         } catch( dgd::exception &ex ) {
            BOOST_TEST_MESSAGE("caught: " << ex.what());
            dirty = true;
         }

         BOOST_CHECK( dirty );
      }
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( simple_scope, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   literal<char_type> test_string("Test string. Test 2 string 2.");
   {
      BOOST_TEST_MESSAGE("Test dgd::basic_scope");      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char_type>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         {
            dgd::basic_scope<char_type> logger("scope #1");

            logger << "argc: " << argc << std::endl
                   << test_string.target << std::endl;            
            
            {
               dgd::basic_scope<char_type> logger("scope #2");

               logger << dgd::literal<char_type>("another scope: ").value 
                      << test_string.target << std::endl;

               logger.start("subscope #1");
               logger << dgd::literal<char_type>("subscope: ").value 
                      << test_string.target << std::endl;
               logger.end();
            }

            try {
               dgd::basic_scope<char_type> logger("throw scope");

               logger << "before throw" << std::endl;
               throw dgd::exception("test throw");
               logger << "after throw" << std::endl;
               
             } catch(const std::exception &ex) {
               logger << "caught: " << ex.what();
            }
         }

         unsigned int indent_step = ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char_type>::reset();

         indented_line_glob glob =
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 3 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) * 2 +
            make_glob(arg_scope == 3 && arg_indent == val(3*indent_step)) +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 2 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step) && 
                      arg_text == val(std::string("caught: test throw"))) +
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step));
         
         sane(real_log, indent_step, glob);
      }
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( scope_char, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   {
      BOOST_TEST_MESSAGE("Test dgd::scope");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         {
            dgd::scope logger(dgd_fileline " scope #1");

            logger << "argc: " << argc << std::endl
                   << "hello world" << std::endl;            
            
            {
               dgd::scope logger(dgd_fileline " scope #2");

               logger << "another scope: hello world" << std::endl;

               logger.start(dgd_fileline " subscope #1");
               logger << "subscope: " << std::hex << 0xf00e1234  << std::dec << std::endl;
               logger.end();
            }

            try {
               dgd::scope logger(dgd_fileline " throw scope");

               logger << "before throw" << std::endl;
               throw dgd::exception("test throw");
               logger << "after throw" << std::endl;
               
             } catch(const std::exception &ex) {
               logger << "caught: " << ex.what();
            }
         }

         unsigned int indent_step = ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char>::reset();

         indented_line_glob glob =
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 3 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) * 2 +
            make_glob(arg_scope == 3 && arg_indent == val(3*indent_step)) +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 2 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 2 +
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step));

         sane(real_log, indent_step, glob);
      }
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( scope_macros, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   {
      BOOST_TEST_MESSAGE("Test dgd::scope");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         int var1 = 1;
         int var2 = 2;
         int var3 = 3;
         {
            dgd_scope;

            dgd_logger << "var1: " << var1 << std::endl;

            std::string var1 = "Test String";

            dgd_echo(var1);
            dgd_echo(var2);
            dgd_echo(var3);
            dgd_logger << dgd_expand(var1) << std::endl;
            
            {
               dgd_scopev("scope #2");
               
               float var3 = 0.12;

               dgd_logger << "another scope: hello world" << std::endl;
               dgd_echo(var1);
               dgd_echo(var2);
               dgd_echo(var3);

               dgd_logger.start(dgd_fileline " subscope #1");
               dgd_logger << "subscope: hello world" << std::endl;
               dgd_logger.end();
            }

            try {
               dgd_scopev("throw scope");

               dgd_echo(var1);
               dgd_echo(var2);
               dgd_echo(var3);
               
               dgd_logger << "before throw" << std::endl;
               throw dgd::exception("test throw");
               dgd_logger << "after throw" << std::endl;
               
             } catch(const std::exception &ex) {
               dgd_logger << "caught: " << dgd_expand(ex.what()) << std::endl ;
               dgd_echo(var1);
               dgd_echo(var2);
               dgd_echo(var3);               
            }
         }

         unsigned int indent_step = 
            ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char>::reset();

         indented_line_glob glob =
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 6 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) * 5 +
            make_glob(arg_scope == 3 && arg_indent == val(3*indent_step)) +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 2 +
            make_glob(arg_scope == 2 && arg_indent == val(2*indent_step)) * 4 +
            make_glob(arg_scope == 1 && arg_indent == val(1*indent_step)) * 5 +
            make_glob(arg_scope == 0 && arg_indent == val(0*indent_step));

         sane(real_log, indent_step, glob);
      }
   }
}

struct test_struct {
   int a;
   std::string b;
   float c;
};

template<typename char_type>
std::basic_ostream<char_type> &operator <<(std::basic_ostream<char_type> &ostr, 
                                           const test_struct& val)
{
   ostr << std::endl << "[" << dgd::incr
        << val.a << ","
        << val.b << ","
        << val.c << dgd::decr
        << "]";
   return ostr;
}

template<typename char_type>
dgd::channel<char_type> &operator <<(dgd::channel<char_type> &ostr, 
                                     const test_struct& val)
{
   ostr << std::endl << "{" << dgd::incr << std::endl;

   ostr << val.a << std::endl 
        << val.b << std::endl
        << val.c << std::endl;

   ostr << dgd::decr << "}" << std::endl;

   return ostr;
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( expand_macros, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   {
      BOOST_TEST_MESSAGE("Test dgd::scope");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         {
            dgd_scope;

            int x = 100;
            int *p = &x;
            int *n = NULL;
            int *z = p;
            const int* y = &x;
            const int* const yc = &x;

            dgd_logger << dgd_expandr(x) << std::endl
                       << dgd_expandr(p) << std::endl
                       << dgd_expandr(n) << std::endl
                       << dgd_expandr(*z) << std::endl
                       << dgd_expandr(y) << std::endl
                       << dgd_expandr(yc) << std::endl;
            {
               dgd_scope;

               test_struct ts;

               ts.a = 5;
               ts.b = "hello";
               ts.c = 8.789;

               test_struct *tsp = &ts;

               dgd_echo(ts);
               dgd_logger << dgd_expandr(ts) << std::endl
                          << dgd_expandr(tsp) << std::endl;
               
            }
         }

         unsigned int indent_step = ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char>::reset();

         sane(real_log, indent_step);
      }
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( manip, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   {
      BOOST_TEST_MESSAGE("Test dgd manips");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         int argc = 2;
         char *argv[3];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = NULL;

         dgd::controller<char>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         {
            dgd_scope;

            test_struct ts;
            
            ts.a = 7;
            ts.b = "dgd hello";
            ts.c = 8.9989;
            
            dgd_logger << "ts: " << ts << std::endl;

            std::basic_ostringstream<char_type> ostr;
            
            ostr << "ts: " << ts << std::endl;
            
            BOOST_TEST_MESSAGE("output to basic_ostream: " << ostr.str());
         }

         unsigned int indent_step = 
            ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char>::reset();

         sane(real_log, indent_step);
      }
   }
}

#define TEST_SCOPE_A 0x00000001
#define TEST_SCOPE_B 0x00000002
#define TEST_SCOPE_C 0x00000004
#define TEST_SCOPE_D 0x00000008

#undef dgd_default_scope_filter
#define dgd_default_scope_filter TEST_SCOPE_A

BOOST_TEST_CASE_TEMPLATE_FUNCTION( scope_filter, char_type )
{
   namespace fs = boost::filesystem;
   using namespace dgd::test;
   using namespace boost::phoenix;

   temp_file log("controller_test", "log");

   {
      BOOST_TEST_MESSAGE("Test dgd scope filtering");
      
      {
         std::string trace_log_arg = 
            std::string("--trace-log=") + log.path().string();

         std::ostringstream ostr;
         ostr << "--trace-filter=" << (int)(TEST_SCOPE_A | 
                                            TEST_SCOPE_B |
                                            TEST_SCOPE_C);
         std::string filter_arg = ostr.str();

         int argc = 3;
         char *argv[4];
         argv[0] = (char*)"exe";
         argv[1] = (char*)trace_log_arg.c_str();
         argv[2] = (char*)filter_arg.c_str();
         argv[3] = NULL;

         dgd::controller<char>::init(argc, argv);
         dgd::controller<char_type> *ctrl = dgd::controller<char_type>::get();

         BOOST_CHECK(ctrl != NULL);
         BOOST_TEST_MESSAGE("controller error " << ctrl->last_error());
         BOOST_CHECK(ctrl->last_error().empty());

         {
            dgd_scope;

            dgd_logger << "inside scope A #1" << std::endl; 

            test_struct ts;
            
            ts.a = 7;
            ts.b = "dgd hello";
            ts.c = 8.9989;
            
            dgd_logger << "ts: " << ts << std::endl;

#undef dgd_default_scope_filter
#define dgd_default_scope_filter TEST_SCOPE_B

            {
               dgd_scope;

               dgd_logger << "inside scope B #1" << std::endl;               
            }

            {
               dgd_scopef(TEST_SCOPE_B);

               dgd_logger << "inside scope B #2" << std::endl;               
            }

            {
               dgd_scopefv(TEST_SCOPE_B, "scope B #3");

               dgd_logger << "inside scope B #3" << std::endl; 
               
               {
                  dgd_scopef(TEST_SCOPE_C);
                  dgd_logger << "inside scope C #1" << std::endl; 
               }
            }

            {
               dgd_scopef(TEST_SCOPE_C);
               dgd_logger << "inside scope C #2" << std::endl; 

               {
                  dgd_scopef(TEST_SCOPE_D);
                  dgd_logger << "inside scope D" << std::endl; 

                  {
                     dgd_scopef(TEST_SCOPE_A);
                     dgd_logger << "inside scope A #2" << std::endl; 

                     {
                        dgd_scope;
                        dgd_logger << "inside scope B #4" << std::endl; 
                     }
                  }

               }
            }

         }

         unsigned int indent_step = 
            ctrl->get_channel().wrapper()->indent_step();
         temp_file real_log(ctrl->get_channel().log_file_name());

         dgd::controller<char>::reset();

         sane(real_log, indent_step);
      }
   }
}

bool init_test()
{
   typedef boost::mpl::list<char, wchar_t> char_types;

   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( simple_controller, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( controller_in_thread, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( controller_help, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( simple_scope, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( scope_char, boost::mpl::list<char> ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( scope_macros, boost::mpl::list<char> ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( expand_macros, boost::mpl::list<char> ));
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( scope_filter, boost::mpl::list<char> ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( manip, boost::mpl::list<char> ) );
 
   return true;
}

int main( int argc, char* argv[] )
{
    return ::boost::unit_test::unit_test_main( &init_test, argc, argv );
}


// 
// Local Variables:
// compile-command: "make dg_test_controller.obj"
// End:
//


