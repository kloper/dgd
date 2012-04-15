// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_temp_file.h -- temporary file that is deleted in instance desctructor
// 

#ifndef _dg_temp_file_h_
#define _dg_temp_file_h_

namespace dgd {
namespace test {

class temp_file
{
  public:

   temp_file(const std::string &basename, const std::string& ext);
   temp_file(const boost::filesystem::path& path);
   ~temp_file();

   boost::filesystem::path path() const { return m_temp_path; }

  private:
   
   std::string m_basename;
   std::string m_ext;
   boost::filesystem::path m_temp_path;
};

} // end of namespace test
} // end of namespace dgd


#endif /* _dg_temp_file_h_ */

/* 
 * Local Variables:
 * compile-command: "make dg_temp_file.obj"
 * End:
 */


