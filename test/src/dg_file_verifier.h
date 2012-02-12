// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_file_verifier.h -- verify sanity of log file created by dgd
// 

#ifndef _dg_file_verifier_h_
#define _dg_file_verifier_h_

namespace dgd {
namespace test {

class file_verifier {
public:
   file_verifier() {}

   file_verifier(const boost::filesystem::path& file) :
      m_file(file)
   {
   }

   bool sane(int indent_step) const;
   bool sane(int indent_step, indented_file &parsed_result) const;
   bool sane(int indent_step, indented_line_glob &glob) const;

   void set(const boost::filesystem::path& file) { 
      m_file = file;
      m_content.clear();
   }

   std::streamsize load();
   
   const std::string &content() const { return m_content; }

private:
   boost::filesystem::path m_file;
   std::string m_content;
};

void sane(const temp_file& log, unsigned int indent_step, indented_line_glob &glob);
void sane(const temp_file& log, unsigned int indent_step);


} // end of namespace test
} // end of namespace dgd

#endif /* _dg_file_verifier_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_file_verifier.obj"
 * End:
 */


