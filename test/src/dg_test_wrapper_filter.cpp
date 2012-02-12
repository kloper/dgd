// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_test_wrapper_filter.cpp -- test dgd::wrapper_filter
// 

#include <vector>
#include <iterator>

#include <boost/mpl/list.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/variant/recursive_variant.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>

#include <dg_wrapper_filter.h>

#include "dg_test_common.h"
#include "dg_parser.h"
#include "dg_parser_utils.h"
#include "dg_parser_verifier.h"
#include "dg_parser_fixture.h"

BOOST_TEST_CASE_TEMPLATE_FUNCTION( constructors_test, char_type )
{
   {
      dgd::wrapper_filter<char_type> filter;
      BOOST_CHECK_EQUAL( filter.line(), (unsigned int)0 );
      BOOST_CHECK_EQUAL( filter.column(), (unsigned int)0 );
   }
}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( unchained_filter_test, char_type )
{
   namespace io = boost::iostreams;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   
   {
      BOOST_TEST_MESSAGE("Simple output line");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);
      
      outstream << test_string.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();

      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
      
      BOOST_CHECK( str == test_string.target );
   }

   {
      BOOST_TEST_MESSAGE("Number of  output lines");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);
      
      outstream << 9 << test_string.target;
      outstream << 9 << test_string.target; 
      outstream << 9 << test_string.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();

      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
      
      BOOST_CHECK_EQUAL( std::count( str.begin(), str.end(), '9'), 3 );
   }

}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( word_wrap_test, char_type )
{
   namespace io = boost::iostreams;
   using namespace dgd::test;
   using namespace boost::phoenix;

   parser_fixture fixture;

   literal<char_type> test_string("Test string. Long-not-splittable-text. "
                                  "Test 2 string 2. ");
   
   {      
      int max_width = 10;
      BOOST_TEST_MESSAGE("Word wrap with max_width " << max_width);
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.max_width(max_width);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);      

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob = 
         make_glob(arg_scope == 0 && arg_indent + strlen_(arg_text) <= val(max_width)) * 7;
            
      BOOST_CHECK( sane(parsed_result, glob) );
   }

   {
      BOOST_TEST_MESSAGE("Word wrap with max_width at first space pos");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      int max_width = test_string.source.find(' ');

      filter.max_width(max_width);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();

      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );

      indented_line_glob glob = 
         make_glob(arg_scope == 0 && arg_indent + strlen_(arg_text) <= max_width) * 15;
      
      BOOST_CHECK( sane(parsed_result, glob) );
   }

   {
      BOOST_TEST_MESSAGE("Word wrap with max_width at last space pos");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      int max_width = test_string.source.rfind(' ');
      filter.max_width(max_width);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();

      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );
                                           
      indented_line_glob glob = 
         make_glob(arg_scope == 0 && arg_indent + strlen_(arg_text) <= max_width) * 2;
      
      BOOST_CHECK( sane(parsed_result, glob) );       
   }

   {
      BOOST_TEST_MESSAGE("Wrap line containing spaces only");
      literal<char_type> test_string("                                         ");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      int max_width = 5;
      filter.max_width(max_width);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();

      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob = 
         make_glob(arg_indent == val(max_width) && strlen_(arg_text) == 0) * (test_string.target.length() / max_width);

      int last_line_len = test_string.target.length() % max_width;
      if(last_line_len != 0) {
         glob
            (arg_indent == val(last_line_len) && strlen_(arg_text) == 0);
      }
      
      BOOST_CHECK( sane(parsed_result, glob) );       
   }

   {
      BOOST_TEST_MESSAGE("Wrap line containing many space inside only");
      literal<char_type> test_string("Start                                         End");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      int max_width = 5;
      filter.max_width(max_width);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();

      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob = 
         make_glob(arg_scope == 0 && arg_indent + strlen_(arg_text) <= max_width) * 10;
      
      BOOST_CHECK( sane(parsed_result, glob) );       
   }

}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( eol_test, char_type )
{
   namespace io = boost::iostreams;

   std::ostringstream ostr;
   ostr << "Line 1. Sample Text #1" << std::endl 
        << "Line 2. Sample Text #2" << std::endl
        << "Line 3. Sample Text #3" << std::endl;
      
   literal<char_type> test_string(ostr.str());
   
   {
      BOOST_TEST_MESSAGE("Simple eol propagation");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      io::filtering_stream<io::output, char_type> outstream;
      

      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
                                                  
      BOOST_CHECK( str == test_string.target );
   }

   {
      BOOST_TEST_MESSAGE("Eol propagation with max_width "
                         "wider than line length");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.max_width(25);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
                                                  
      BOOST_CHECK( str == test_string.target );
   }

   {
      BOOST_TEST_MESSAGE("Eol propagation with max_width "
                         "exactly as line length");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.max_width(test_string.source.find('\n'));
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
                                                  
      BOOST_CHECK( str == test_string.target );
   }

   {
      BOOST_TEST_MESSAGE("Eol propagation with max_width "
                         "shorter than line length and word wrap false");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.max_width(10);
      filter.word_wrap(false);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
                          
      BOOST_CHECK_EQUAL( std::count(str.begin(), str.end(), '\n'), 
                         test_string.source.length() / filter.max_width() +
                         std::count(test_string.source.begin(), 
                                    test_string.source.end(), '\n') ); 
   }

   {
      BOOST_TEST_MESSAGE("Eol propagation with max_width "
                         " shorter than line length and wrap false");

      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.max_width(10);
      filter.wrap(false);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << test_string.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
                                                  
      BOOST_CHECK( str == test_string.target );
   }

}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( indent_test, char_type )
{
   namespace io = boost::iostreams;
   using namespace dgd::test;
   using namespace boost::phoenix;

   parser_fixture fixture;

   std::ostringstream multiline_ostr;
   multiline_ostr << "Line 1. Sample Text #1" << std::endl 
                  << "Line 2. Sample Text #2" << std::endl
                  << "Line 3. Sample Text #3" << std::endl;
      
   literal<char_type> multiline(multiline_ostr.str());

   std::ostringstream single_line_ostr;
   single_line_ostr << "Line 1. Sample Text #1";
      
   literal<char_type> single_line(single_line_ostr.str());
   
   {
      BOOST_TEST_MESSAGE("Simple indent");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      filter.incr_indent();
      io::filtering_stream<io::output, char_type> outstream;
      

      outstream.push(filter);
      outstream.push(string_outstream);     

      outstream << multiline.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      
      std::string result = wstr2str(str);
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob = 
         make_glob(arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width())) * 3;
      
      BOOST_CHECK( sane(parsed_result, glob) );       
   }

   {
      BOOST_TEST_MESSAGE("Simple increasing indent");

      typedef dgd::wrapper_filter<char_type> filter_type; 
      std::basic_ostringstream<char_type> string_outstream;
      filter_type filter;
      io::filtering_stream<io::output, char_type> outstream;     

      outstream.push(filter);
      outstream.push(string_outstream);     

      filter_type* specific_filter = outstream.component<filter_type>(0);

      specific_filter->incr_indent();
      outstream << single_line.target << std::endl;
      specific_filter->incr_indent();
      outstream << single_line.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      std::string result = wstr2str(str);

      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob = 
         make_glob
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(2*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         ;
      
      BOOST_CHECK( sane(parsed_result, glob) );     
   }

   {
      BOOST_TEST_MESSAGE("Multiline increasing indent");

      typedef dgd::wrapper_filter<char_type> filter_type; 
      std::basic_ostringstream<char_type> string_outstream;
      filter_type  filter;
      io::filtering_stream<io::output, char_type> outstream;     

      outstream.push(filter);
      outstream.push(string_outstream);     

      filter_type* specific_filter = outstream.component<filter_type>(0);

      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      
      std::basic_string<char_type> str = string_outstream.str();
      std::string result = wstr2str(str);

      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob =
         make_glob
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(2*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width())) 
         (arg_indent ==  val(2*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width())) 
         (arg_indent ==  val(2*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width())) 
         ;
      
      BOOST_CHECK( sane(parsed_result, glob) );     
   }

   {
      BOOST_TEST_MESSAGE("Multiline decreasing indent");

      typedef dgd::wrapper_filter<char_type> filter_type; 
      std::basic_ostringstream<char_type> string_outstream;
      filter_type  filter;
      io::filtering_stream<io::output, char_type> outstream;     

      outstream.push(filter);
      outstream.push(string_outstream);     

      filter_type* specific_filter = outstream.component<filter_type>(0);

      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->decr_indent();      
      outstream << multiline.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();
      std::string result = wstr2str(str);

      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob =
         make_glob
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         (arg_indent ==  val(0*filter.indent_step()) && arg_indent + strlen_(arg_text) <= val(filter.max_width()))
         ;
      
      BOOST_CHECK( sane(parsed_result, glob) );     
   }

}

BOOST_TEST_CASE_TEMPLATE_FUNCTION( min_width_test, char_type )
{
   namespace io = boost::iostreams;
   using namespace dgd::test;
   using namespace boost::phoenix;

   parser_fixture fixture;

   std::ostringstream multiline_ostr;
   multiline_ostr << "Line 1. Sample Text #1" << std::endl 
                  << "Line 2. Sample Text #2" << std::endl
                  << "Line 3. Sample Text #3" << std::endl;
      
   literal<char_type> multiline(multiline_ostr.str());

   std::ostringstream single_line_ostr;
   single_line_ostr << "Line 1. Sample Text #1";
      
   literal<char_type> single_line(single_line_ostr.str());
   
   {
      BOOST_TEST_MESSAGE("Minimum line width will allow only 2 indents");

      typedef dgd::wrapper_filter<char_type> filter_type; 
      std::basic_ostringstream<char_type> string_outstream;
      filter_type filter;
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      filter_type* specific_filter = outstream.component<filter_type>(0);

      unsigned int indent_step = specific_filter->indent_step();
      unsigned int max_width = indent_step * 2 + single_line.source.length();
      unsigned int min_width = max_width - (indent_step * 2 + 3);

      specific_filter->max_width( max_width );
      specific_filter->min_width( min_width );

      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();
      std::string result = wstr2str(str);
      
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob =
         make_glob(arg_indent ==  val(0*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 3 +
         make_glob(arg_indent >=  val(1*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 3 +
         make_glob(arg_indent >=  val(2*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 6
         ;
      
      BOOST_CHECK( sane(parsed_result, glob) );     
   }

   {
      BOOST_TEST_MESSAGE("Minimum line width will allow only 2 indents, "
                         "max width will split each line");

      typedef dgd::wrapper_filter<char_type> filter_type; 
      std::basic_ostringstream<char_type> string_outstream;
      dgd::wrapper_filter<char_type> filter;
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);     

      filter_type* specific_filter = outstream.component<filter_type>(0);

      unsigned int indent_step = specific_filter->indent_step();
      unsigned int max_width = indent_step * 2 + single_line.source.length() / 2;
      unsigned int min_width = max_width - (indent_step * 2 + 3);

      specific_filter->max_width( max_width );
      specific_filter->min_width( min_width );

      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;
      specific_filter->incr_indent();
      outstream << multiline.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();
      std::string result = wstr2str(str);
      
      BOOST_TEST_MESSAGE("result: " << result);

      dgd::test::indented_file parsed_result;
      BOOST_CHECK( dgd::test::parse(result, parsed_result) );
      BOOST_TEST_MESSAGE("parsed result: " << dgd::test::str(parsed_result));
      BOOST_CHECK( sane(filter.indent_step(), parsed_result) );

      indented_line_glob glob =
         make_glob(arg_indent ==  val(0*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 3 +
         make_glob(arg_indent >=  val(1*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 6 +
         make_glob(arg_indent >=  val(2*indent_step) && arg_indent + strlen_(arg_text) <= val(max_width)) * 18;
      
      BOOST_CHECK( sane(parsed_result, glob) );  
   
      BOOST_TEST_MESSAGE( "Glob pos: " << glob.pos() );
   }
}

bool init_test()
{
   typedef boost::mpl::list<char, wchar_t> char_types;

   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( constructors_test, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( unchained_filter_test, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( word_wrap_test, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( eol_test, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( indent_test, char_types ) );
   ::boost::unit_test::framework::master_test_suite().
        add( BOOST_TEST_CASE_TEMPLATE( min_width_test, char_types ) );
 
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


