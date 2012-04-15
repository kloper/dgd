// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_temp_file.cpp -- temporary file that is deleted in instance desctructor
// 

#include <string>

#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "dg_temp_file.h"

namespace dgd {
namespace test {

temp_file::temp_file(const std::string &basename, const std::string& ext) :
   m_basename(basename),
   m_ext(ext)
{
   namespace fs = boost::filesystem;
   
   fs::path pattern = 
      fs::temp_directory_path() / (basename + "-%%%%%" + "." + ext);
   m_temp_path = fs::unique_path(pattern);      
}

temp_file::temp_file(const boost::filesystem::path& path)
{
   boost::filesystem::path p(path);
//   m_ext = p.extension();
   p.replace_extension();
   m_basename = p.string();
   m_temp_path = path;
}

temp_file::~temp_file()
{
   namespace fs = boost::filesystem;
   try
   {
      if(fs::exists(m_temp_path)) {
         fs::remove(m_temp_path);
      }
   }
   catch(const fs::filesystem_error& ex)
   {
      BOOST_TEST_MESSAGE("Failed to remove " << m_temp_path 
                         << ". Unexpected exception: " << ex.what());
   }
}

} // end of namespace test
} // end of namespace dgd


// 
// Local Variables:
// compile-command: "make dg_temp_file.obj"
// End:
//


