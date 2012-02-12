// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_verifier.cpp -- utils for parser output line-by-line verification
// 

#include <iostream>
#include <sstream>
#include <vector>

#include <boost/typeof/typeof.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function/adapt_function.hpp>
#include <boost/phoenix/statement/if.hpp>

#include "dg_parser.h"
#include "dg_parser_verifier.h"

namespace dgd {
namespace test {

boost::phoenix::function<strlen_impl> strlen_;

template<typename Verifier>
class indented_block_verifier
{
public:
   indented_block_verifier(int nesting_level, 
                           Verifier& verifier) :
      m_nesting_level(nesting_level),
      m_verifier(verifier)
   {
   }
      
   bool operator()(indented_block const& block) const;

private:
   int m_nesting_level;
   Verifier& m_verifier;
};

template<typename Verifier>
class indented_block_node_verifier: public boost::static_visitor<bool>
{
public:
   indented_block_node_verifier(int nesting_level, 
                                Verifier& verifier) :
      m_nesting_level(nesting_level),
      m_verifier(verifier)
   {
   }
   
   bool operator()(indented_block const& block) const
   {
      return indented_block_verifier<Verifier>(m_nesting_level, m_verifier)(block);
   }
      
   bool operator()(indented_line const& line) const
   {
      return m_verifier(m_nesting_level, line);
   }
      
private:
   int m_nesting_level;
   Verifier& m_verifier;
};

BOOST_PHOENIX_ADAPT_FUNCTION(bool, apply_visitor, boost::apply_visitor, 2)

template<typename Verifier>
bool indented_block_verifier<Verifier>::operator()(indented_block const& block) const
{
   using namespace boost::phoenix;
   using namespace boost::phoenix::arg_names;

   bool rc;

   rc = m_verifier(m_nesting_level, block.header);
   if(!rc) return false;

   indented_block_node_verifier<Verifier> visitor(m_nesting_level+1, m_verifier);

   std::for_each(block.children.begin(), block.children.end(),
                 if_ (ref(rc)) [
                    ref(rc) &= apply_visitor( boost::phoenix::ref(visitor) , arg1 )
                 ]
   );

   if(!rc) return false;

   rc &= m_verifier(m_nesting_level, block.footer);

   return rc;
}

class indented_line_verifier: public boost::static_visitor<bool>
{
public:
   indented_line_verifier(int scope, const indented_line& line) :
      m_scope(scope),
      m_line(line)
   {}

   template<typename Callable>
   bool operator()(const Callable& callable) const
   {
      return callable(m_scope, m_line.indent, m_line.text);
   }

private:
   int m_scope;
   const indented_line &m_line;
};

boost::phoenix::expression::argument<1>::type arg_scope;
boost::phoenix::expression::argument<2>::type arg_indent;
boost::phoenix::expression::argument<3>::type arg_text;

indented_line_glob::indented_line_glob()
{
   m_current = m_sequence.begin();
   m_end = m_sequence.end();
}

indented_line_glob::indented_line_glob(const indented_line_glob& peer)
{
   m_sequence.clear();

   for(verifier_sequence_const_iterator iter = peer.begin(); iter != peer.end(); ++iter) {
      m_sequence.push_back(*iter);
   }
   m_current = m_sequence.begin();
   m_end = m_sequence.end();
}

indented_line_glob &indented_line_glob::operator()(const verifier_dictionary_t& v)
{
   m_sequence.push_back(v);
   m_current = m_sequence.begin();
   m_end = m_sequence.end();
   return *this;
}

bool indented_line_glob::operator()(int scope, indented_line const& line)
{ 
   if(m_current == m_end) 
      return false;
   
   indented_line_verifier visitor(scope, line);
   return boost::apply_visitor(visitor, *m_current++);
}

indented_line_glob make_glob(const verifier_dictionary_t& v)
{
   indented_line_glob c;   
   c(v);
   return c;
}

indented_line_glob operator + (const indented_line_glob& a, const indented_line_glob& b)
{
   indented_line_glob c;

   for(verifier_sequence_const_iterator iter = a.begin(); iter != a.end(); ++iter)
   {
      c(*iter);
   }

   for(verifier_sequence_const_iterator iter = b.begin(); iter != b.end(); ++iter)
   {
      c(*iter);
   }

   return c;
}

indented_line_glob operator * (const indented_line_glob& a, int n)
{
   indented_line_glob c;
   
   for(int i = 0; i < n; ++i)
   {
      for(verifier_sequence_const_iterator iter = a.begin(); iter != a.end(); ++iter)
      {
         c(*iter);
      }
   }

   return c;
}

bool sane(const indented_file& f, indented_line_glob& glob)
{
   using namespace boost::phoenix;
   using namespace boost::phoenix::arg_names;

   bool rc = true;

   indented_block_node_verifier<indented_line_glob> visitor(0, glob);
   std::for_each(
      f.begin(), f.end(),
      if_(ref(rc)) [ 
         ref(rc) &= apply_visitor( boost::phoenix::ref(visitor), arg1 )
      ] 
   );

   rc &= glob.complete();
   return rc;
}

} // end of namespace test
} // end of namespace dgd


/* 
 * Local Variables:
 * compile-command: "make dg_parser_verifier.obj"
 * End:
 */


