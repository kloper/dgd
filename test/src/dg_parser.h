// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser.h -- interface for the dgd output parser
// 

#ifndef _dg_parser_h_
#define _dg_parser_h_

namespace dgd {

namespace test {

struct indented_line
{
      int indent;
      std::string text;
};

struct indented_block;

typedef boost::variant<boost::recursive_wrapper<indented_block>, 
                       indented_line> indented_block_node;

typedef std::vector<indented_block_node> indented_file;

struct indented_block
{
   indented_line header;
   indented_file children;
   indented_line footer;
};

extern boost::iostreams::filtering_ostream debug_ostream;
extern boost::iostreams::null_sink devnull;

bool parse( const std::string& input, indented_file &result );

} // end of namespace test
} // end of namespace dgd

#endif /* _dg_parser_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_parser.obj"
 * End:
 */


