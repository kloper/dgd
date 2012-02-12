// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_parser_verifier.h -- utils for test parser verifier
// 

#ifndef _dg_parser_verifier_h_
#define _dg_parser_verifier_h_

namespace dgd {
namespace test {

extern boost::phoenix::expression::argument<1>::type arg_scope;
extern boost::phoenix::expression::argument<2>::type arg_indent;
extern boost::phoenix::expression::argument<3>::type arg_text;
extern unsigned int dummy_uint;
extern std::string dummy_str;

struct strlen_impl
{
    typedef unsigned int result_type;

    template <typename char_type>
    unsigned int operator()(const std::basic_string<char_type>& str) const
    {
       return str.length();
    }
};

extern boost::phoenix::function<strlen_impl> strlen_;


typedef boost::variant<
   BOOST_TYPEOF(boost::phoenix::val(true)),
   BOOST_TYPEOF(arg_indent == 0),
   BOOST_TYPEOF(arg_scope == 0),
   BOOST_TYPEOF(arg_text == boost::phoenix::ref(dummy_str)),
   BOOST_TYPEOF(arg_indent == boost::phoenix::ref(dummy_uint)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent == boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_scope == boost::phoenix::val(dummy_uint) && arg_indent == boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent == boost::phoenix::val(dummy_uint) && arg_text == boost::phoenix::val(dummy_str)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent >= boost::phoenix::val(dummy_uint) && arg_text == boost::phoenix::val(dummy_str)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent > boost::phoenix::val(dummy_uint) && arg_text == boost::phoenix::val(dummy_str)),
   BOOST_TYPEOF(arg_scope == 0 && arg_text == boost::phoenix::val(dummy_str)),
   BOOST_TYPEOF(arg_indent == 0 && arg_text == boost::phoenix::val(dummy_str)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent + strlen_(arg_text) <= 0),
   BOOST_TYPEOF(arg_indent == 0 && strlen_(arg_text) == 0),
   BOOST_TYPEOF(arg_indent == 0 && strlen_(arg_text) == boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_indent == boost::phoenix::val(dummy_uint) && arg_indent + strlen_(arg_text) <= boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_indent >= boost::phoenix::val(dummy_uint) && arg_indent + strlen_(arg_text) <= boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_indent <= boost::phoenix::val(dummy_uint) && arg_indent + strlen_(arg_text) <= boost::phoenix::val(dummy_uint)),
   BOOST_TYPEOF(arg_scope == 0 && arg_indent == boost::phoenix::val(dummy_uint) && arg_indent + strlen_(arg_text) <= boost::phoenix::val(dummy_uint))
> verifier_dictionary_t;

typedef std::vector<verifier_dictionary_t> verifier_sequence_t;
typedef typename verifier_sequence_t::iterator verifier_sequence_iterator;
typedef typename verifier_sequence_t::const_iterator verifier_sequence_const_iterator;

class indented_line_glob
{
public:
   indented_line_glob();
   indented_line_glob(const indented_line_glob& peer);

   indented_line_glob &operator()(const verifier_dictionary_t& v);
   
   bool operator()(int scope, indented_line const& line);   

   bool complete() const { return m_current == m_end; }

   verifier_sequence_const_iterator begin() const { return m_sequence.begin(); }
   verifier_sequence_const_iterator end() const { return m_sequence.end(); } 

   int pos() const { return std::distance(m_sequence.begin(), m_current); }
   int rpos() const { return std::distance(m_current, m_sequence.end()); }

private:
   verifier_sequence_t              m_sequence;
   verifier_sequence_const_iterator m_current;
   verifier_sequence_const_iterator m_end;
};

indented_line_glob make_glob(const verifier_dictionary_t& v);

indented_line_glob operator + (const indented_line_glob& a, const indented_line_glob& b);
indented_line_glob operator * (const indented_line_glob& a, int n);

bool sane(const indented_file& f, indented_line_glob& glob);

} // end of namespace test
} // end of namespace dgd

#endif /* _dg_parser_verifier_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_parser_verifier.obj"
 * End:
 */


