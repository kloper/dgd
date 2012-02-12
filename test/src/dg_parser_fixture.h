// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_fixture.h -- parser fixture
// 

#ifndef _dg_parser_fixture_h_
#define _dg_parser_fixture_h_

namespace dgd {
namespace test {

class parser_fixture
{
  public:

   parser_fixture()
   {
      dgd::test::debug_ostream.reset();
      dgd::test::debug_ostream.push(m_ostr);
   }

   ~parser_fixture()
   {
      BOOST_TEST_MESSAGE( "Parser output: " );
      BOOST_TEST_MESSAGE( m_ostr.str() );
      dgd::test::debug_ostream.reset();
      dgd::test::debug_ostream.push(devnull);
   }
  private:
   
   std::ostringstream m_ostr;
};

} // end of namespace test
} // end of namespace dgd

#endif /* _dg_parser_fixture_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_parser_utils.obj"
 * End:
 */


