// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dgd_test_journal_filter.cpp -- unit tests for dgd::journal_filter
// 

#include <boost/mpl/list.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <boost/iostreams/concepts.hpp> 
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>

#include <dg_journal_filter.h>

#include "dg_test_common.h"

BOOST_TEST_CASE_TEMPLATE_FUNCTION( unchained_filter_test, char_type )
{
   namespace io = boost::iostreams;

   literal<char_type> test_string("Test string. Test 2 string 2.");
   
   {
      BOOST_TEST_MESSAGE("Simple output line");
      std::basic_ostringstream<char_type> string_outstream;
      dgd::journal_filter<char_type> filter("test_log", 10);
      io::filtering_stream<io::output, char_type> outstream;
      
      outstream.push(filter);
      outstream.push(string_outstream);
      
      outstream << test_string.target << std::flush;

      std::basic_string<char_type> str = string_outstream.str();

      BOOST_TEST_MESSAGE("result: " << wstr2str(str));
      
      BOOST_CHECK( str == test_string.target );
   }
}

bool init_test()
{
   typedef boost::mpl::list<char, wchar_t> char_types;

   ::boost::unit_test::framework::master_test_suite().
       add( BOOST_TEST_CASE_TEMPLATE( unchained_filter_test, char_types ) );
 
   return true;
}

int main( int argc, char* argv[] )
{
    return ::boost::unit_test::unit_test_main( &init_test, argc, argv );
}

// 
// Local Variables:
// compile-command: "make dgd_test_journal_filter.obj"
// End:
//


