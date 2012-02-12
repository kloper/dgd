// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_test_parser.cpp -- test dgd::test::parse()
// 

#include <vector>
#include <sstream>

#include <boost/mpl/list.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>

#include "dg_test_common.h"
#include "dg_parser.h"
#include "dg_parser_utils.h"
#include "dg_parser_verifier.h"
#include "dg_parser_fixture.h"

void parse_single_line()
{
   using namespace dgd::test;
   using namespace boost::phoenix;

   parser_fixture fixture;

   std::string test_string("Test string.");
   
   {
      BOOST_TEST_MESSAGE("Parsing a string without identation");

      unsigned int indent_step = 4;
      indented_file parsed_result;

      BOOST_CHECK( parse(test_string, parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step) && arg_text == val(test_string)) 
                   )
      );
   }
}

void parse_single_block()
{
   using namespace dgd::test;
   using namespace boost::phoenix;

   unsigned int indent_step = 4;

   {
      parser_fixture fixture;
      std::string test_string(
         "{ header\n"
         "    text\n"
         "} footer\n"
      );
   
      BOOST_TEST_MESSAGE("Parsing input:");
      BOOST_TEST_MESSAGE(test_string);

      indented_file parsed_result;

      BOOST_CHECK( parse(test_string, parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                   )
      );
   }

   {
      parser_fixture fixture;
      std::string test_string(
         "{ header\n"
         "    text\n"
         " } footer\n"
      );
   
      BOOST_TEST_MESSAGE("Parsing input:");
      BOOST_TEST_MESSAGE(test_string);

      indented_file parsed_result;

      BOOST_CHECK( parse(test_string, parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( !sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 0 && arg_indent == val(1u)) 
                   )
      );
   }

   {
      parser_fixture fixture;
      std::string last("footer");
      std::string test_string(
         "{ header\n"
         "    text\n"
         "}" + last
      );
   
      BOOST_TEST_MESSAGE("Parsing input:");
      BOOST_TEST_MESSAGE(test_string);

      indented_file parsed_result;

      BOOST_CHECK( parse(test_string, parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 0 && arg_indent == val(0*indent_step) && arg_text == val(last)) 
                   )
      );
   }

   {
      parser_fixture fixture;
      std::string test_string(
         "{ header {\n"
         "    text { \n"
         "} } footer\n"
      );
   
      BOOST_TEST_MESSAGE("Parsing input:");
      BOOST_TEST_MESSAGE(test_string);

      indented_file parsed_result;

      BOOST_CHECK( parse(test_string, parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                   )
      );

   }

}

void parse_complex_block()
{
   using namespace dgd::test;
   using namespace boost::phoenix;

   unsigned int indent_step = 4;

   std::string str1("str1");
   std::string str2("str2");
   std::string str3("str3");
   std::string str4("str4");
   std::string str5("str5");
   std::string str6("str6 {");
   std::string str7("str7 }");
   std::string str8("str5 {}");
   
   {
      parser_fixture fixture;

      std::ostringstream test_string_stream;

      test_string_stream << "{" << str1 << std::endl
                         << "    hello world" << std::endl
                         << "    {" << str2 << std::endl
                         << "        hello world" << std::endl
                         << "        {" << str6 << std::endl
                         << "              " << str8 << std::endl
                         << "        }" << str7 << std::endl
                         << "        " << str7 << std::endl
                         << "        {" << str6 << std::endl
                         << "            " << str8 << std::endl
                         << "        }" << str7 << std::endl
                         << "    }" << str3 << std::endl
                         << "    " << str6 << std::endl
                         << "}";
      
      BOOST_TEST_MESSAGE("Parsing input:");
      BOOST_TEST_MESSAGE(test_string_stream.str());

      indented_file parsed_result;

      BOOST_CHECK( parse(test_string_stream.str(), parsed_result) );

      BOOST_TEST_MESSAGE("Parsed result: ");
      BOOST_TEST_MESSAGE(str(parsed_result));

      BOOST_CHECK_EQUAL( parsed_result.size(), 1);
      BOOST_CHECK( sane(indent_step, parsed_result) );

      indented_line_glob glob;

      BOOST_CHECK( sane(parsed_result,       
                        glob
                        (arg_scope == 0 && arg_indent == val(0*indent_step) && arg_text == val(str1)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step) && arg_text == val(str2)) 
                        (arg_scope == 2 && arg_indent == val(2*indent_step)) 
                        (arg_scope == 2 && arg_indent == val(2*indent_step) && arg_text == val(str6)) 
                        (arg_scope == 3 && arg_indent > val(3*indent_step) && arg_text == val(str8))
                        (arg_scope == 2 && arg_indent == val(2*indent_step) && arg_text == val(str7))  
                        (arg_scope == 2 && arg_indent == val(2*indent_step) && arg_text == val(str7)) 
                        (arg_scope == 2 && arg_indent == val(2*indent_step) && arg_text == val(str6)) 
                        (arg_scope == 3 && arg_indent == val(3*indent_step) && arg_text == val(str8))
                        (arg_scope == 2 && arg_indent == val(2*indent_step) && arg_text == val(str7)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step) && arg_text == val(str3)) 
                        (arg_scope == 1 && arg_indent == val(1*indent_step)) 
                        (arg_scope == 0 && arg_indent == val(0*indent_step)) 
                   )
      );
   }
}

bool init_test()
{
   typedef boost::mpl::list<char, wchar_t> char_types;

   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE( &parse_single_line ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE( &parse_single_block ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE( &parse_complex_block ) );
 
   return true;
}

int main( int argc, char* argv[] )
{
   dgd::test::debug_ostream.push(dgd::test::devnull);
   return ::boost::unit_test::unit_test_main( &init_test, argc, argv );
}

// 
// Local Variables:
// compile-command: "make dg_test_wrapper_filter.obj"
// End:
//


