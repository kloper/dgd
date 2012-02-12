// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser.cpp -- utility parsers for testing dgd output
// 

// #define BOOST_SPIRIT_LEXERTL_DEBUG 1
#define BOOST_SPIRIT_DEBUG_OUT dgd::test::debug_ostream

#include <boost/iostreams/filtering_stream.hpp>
namespace dgd { 
namespace test { 
boost::iostreams::filtering_ostream debug_ostream; 
boost::iostreams::null_sink devnull;
} 
}

#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "dg_parser.h"

using namespace boost::spirit;

namespace boost { 
namespace spirit { 
namespace traits {
    template <typename Iterator>
    struct assign_to_attribute_from_iterators<dgd::test::indented_line, Iterator>
    {
        static void 
        call(Iterator const& first, Iterator const& last, dgd::test::indented_line& attr)
        {
           attr.indent = 0;
           Iterator i = first;
           while(i != last) {
              if(*i == ' ') {
                 attr.indent++;
              } else if( *i == '\t' ) {
                 attr.indent += 4;
              } else {
                 break;
              }
              i++;
           }

           if(i != last && (*i == '{' || *i == '}' || *i == '\n' || *i == '\r'))
              i++;

           attr.text = std::string(i, last);
        }
    };
}}}

BOOST_FUSION_ADAPT_STRUCT(
   dgd::test::indented_line,
   (int, indent)
   (std::string, text)
)

BOOST_FUSION_ADAPT_STRUCT(
   dgd::test::indented_block,
   (dgd::test::indented_line, header)
   (std::vector<dgd::test::indented_block_node>, children)
   (dgd::test::indented_line, footer)
)

namespace dgd {
namespace test {

std::ostream &operator <<(std::ostream &ostr, const indented_line& line)
{
   ostr << "[" << line.indent << "] " << line.text;
   return ostr;
}

enum idf_tokenid {
   tok_eol = lex::min_token_id + 11,
   tok_leftb,
   tok_rightb,
   tok_text
};

template <typename Lexer>
struct indented_file_lexer : lex::lexer<Lexer>
{
   indented_file_lexer()
   { 
      using boost::spirit::lex::_val;
      using boost::phoenix::at_c;
      using boost::phoenix::ref;

      eol = "\r?\n"; 
      left_brace = "[ ]*\\{[^\r\n]*"; 
      right_brace = "[ ]*\\}[^\r\n]*"; 
      text = "[ ]*([^ \r\n{}][^\r\n]*)?"; 

      text.id(tok_text);
      left_brace.id(tok_leftb);
      right_brace.id(tok_rightb);
      eol.id(tok_eol);

      this->self =
         text 
         | left_brace
         | right_brace 
         | eol 
         ;
   }
      
   lex::token_def<indented_line> text, left_brace, right_brace, eol;
};

template <typename Iterator>
struct indented_file_grammar : qi::grammar<Iterator, indented_file()>
{
   template <typename TokenDef>
   indented_file_grammar(TokenDef const& tok) : indented_file_grammar::base_type(file) {
      using boost::phoenix::at_c;
      using boost::phoenix::push_back;
      using qi::debug;

      node.name("node");
      block.name("block");
      file.name("file");
      
      node = 
         tok.eol    [_val = _1]
         | tok.text [_val = _1] >> -tok.eol
         | block    [_val = _1]
         ;

      block =
         tok.left_brace      [at_c<0>(_val) = _1]           >> tok.eol
         >> *node            [push_back(at_c<1>(_val),_1)]  
         >> tok.right_brace  [at_c<2>(_val) = _1]           >> -tok.eol
         ;

      file %=
         *node >> qi::eoi
         ;

      debug(file);
      debug(node);
      debug(block);
   }

   qi::rule<Iterator, indented_file()> file;
   qi::rule<Iterator, indented_block_node()> node;
   qi::rule<Iterator, indented_block()> block;
};

bool parse( const std::string& str, indented_file &result ) {
   typedef lex::lexertl::token<std::string::const_iterator, boost::mpl::vector<indented_line> > token_type;
   typedef lex::lexertl::actor_lexer<token_type> lexer_type;
   indented_file_lexer<lexer_type> idflexer;

   typedef typename indented_file_lexer<lexer_type>::iterator_type idflexer_iterator_type;
   indented_file_grammar<idflexer_iterator_type> idfgrammar(idflexer);

   std::string::const_iterator begin_iter = str.begin();
   std::string::const_iterator end_iter = str.end();

   idflexer_iterator_type idflex_first = idflexer.begin(begin_iter, end_iter);
   idflexer_iterator_type idflex_last = idflexer.end();

   bool rc = qi::parse(idflex_first, idflex_last, idfgrammar,  result);
   return rc;
}

} // end of namespace test
} // end of namespace dgd



// 
// Local Variables:
// compile-command: "make dg_parser.obj"
// End:
//


