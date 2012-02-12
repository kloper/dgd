// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_str.cpp -- utils for test parser pretty printing
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

class indented_block_printer
{
   public:
      indented_block_printer(std::ostream &ostr, int nesting_level) :
         m_ostr(ostr),
         m_nesting_level(nesting_level)
      {
      }
      
      void operator()(indented_block const& block) const;

    private:
      std::ostream &m_ostr;
      int m_nesting_level;
};

class indented_block_node_printer : public boost::static_visitor<>
{
   public:
      indented_block_node_printer(std::ostream &ostr, int nesting_level = 0) :
         m_ostr(ostr),
         m_nesting_level(nesting_level)
      {
      }
      
      void operator()(indented_block const& block) const
      {
         indented_block_printer(m_ostr, m_nesting_level)(block);
      }
      
      void operator()(indented_line const& line) const
      {
         m_ostr << "[" << m_nesting_level << "," << line.indent << "]:" 
                << line.text << std::endl;
      }
   
   private:
      std::ostream &m_ostr;
      int m_nesting_level;
};

void indented_block_printer::operator()(indented_block const& block) const
{
   m_ostr << "[" << m_nesting_level << "," << block.header.indent << "]:" 
          << block.header.text << std::endl;
   
   indented_block_node_printer visitor(m_ostr, m_nesting_level+1);
   std::for_each(block.children.begin(), block.children.end(),
                 boost::apply_visitor(visitor));
   
   m_ostr << "[" << m_nesting_level << "," << block.footer.indent << "]:" 
          << block.footer.text << std::endl;
}

std::string str(const indented_file& f) 
{
   std::ostringstream ostr;
   indented_block_node_printer visitor(ostr, 0);
   std::for_each(f.begin(), f.end(), boost::apply_visitor(visitor));

   return ostr.str();
}

} // end of namespace test
} // end of namespace dgd

std::ostream &operator <<(std::ostream &ostr, const dgd::test::indented_file& f)
{
   using namespace dgd::test;

   indented_block_node_printer visitor(ostr, 0);
   std::for_each(f.begin(), f.end(), boost::apply_visitor(visitor));
   return ostr;
}

/* 
 * Local Variables:
 * compile-command: "make dg_parser_str.obj"
 * End:
 */


