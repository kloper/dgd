// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_sane.cpp -- utils for parser output sanity checking
// 

#include <iostream>
#include <sstream>
#include <vector>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "dg_parser.h"
#include "dg_parser_utils.h"

namespace dgd {
namespace test {

class indented_block_sanity_checker
{
   public:
      indented_block_sanity_checker(int indent_step, int nesting_level) :
         m_indent_step(indent_step),
         m_nesting_level(nesting_level)
      {
      }
      
      bool operator()(indented_block const& block) const;

    private:
      int m_indent_step;
      int m_nesting_level;
};

class indented_block_node_sanity_checker : public boost::static_visitor<bool>
{
   public:
      indented_block_node_sanity_checker(int indent_step, int nesting_level = 0) :
         m_indent_step(indent_step),
         m_nesting_level(nesting_level)
      {
      }
      
      bool operator()(indented_block const& block) const
      {
         return indented_block_sanity_checker(m_indent_step, m_nesting_level)(block);
      }
      
      bool operator()(indented_line const& line) const
      {
         return (line.indent >= m_indent_step * m_nesting_level);
      }
   
   private:
      int m_indent_step;
      int m_nesting_level;
};

bool indented_block_sanity_checker::operator()(indented_block const& block) const
{
   bool rc;
   
   rc = (block.header.indent == block.footer.indent) &&
        (block.header.indent >= m_indent_step * m_nesting_level) &&
        (block.footer.indent >= m_indent_step * m_nesting_level) &&
        sane(m_indent_step+1, block.children);
   
   return rc;
}

bool sane(int indent_step, const indented_file& f)
{
   bool rc = true;

   for(indented_file::const_iterator iter = f.begin();
       iter != f.end();
       ++iter)
   {
      indented_block_node_sanity_checker visitor(indent_step, 0);
      rc &= boost::apply_visitor(visitor, *iter);
   }

   return rc;
}

} // end of namespace test
} // end of namespace dgd


/* 
 * Local Variables:
 * compile-command: "make dg_parser_sane.obj"
 * End:
 */


