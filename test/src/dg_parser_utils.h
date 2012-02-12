// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_utils.h -- utils for test parser
// 

#ifndef _dg_parser_utils_h_
#define _dg_parser_utils_h_

namespace dgd {
namespace test {

std::string str(const indented_file& f);
bool sane(int indent_step, const indented_file& f);

} // end of namespace test
} // end of namespace dgd

std::ostream &operator <<(std::ostream &ostr, const dgd::test::indented_file& f);

#endif /* _dg_parser_utils_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_parser_utils.obj"
 * End:
 */


