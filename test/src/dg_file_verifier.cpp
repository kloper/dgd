// 
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dg_file_verifier.cpp -- verify sanity of log file created by dgd
// 

#include <string>

#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/copy.hpp>

#include <boost/variant/recursive_variant.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/operator.hpp>

#include "dg_parser.h"
#include "dg_parser_verifier.h"
#include "dg_parser_utils.h"

#include "dg_temp_file.h"
#include "dg_file_verifier.h"

namespace dgd {
namespace test {

class string_sink {
public:
   typedef typename std::string::value_type  char_type;
   typedef boost::iostreams::sink_tag        category;

   string_sink(std::string& str) : m_string(str) 
   { 
   }

   std::streamsize write(const char_type* s, std::streamsize n)
   {
        m_string.insert(m_string.end(), s, s + n);
        return n;
   }
    
private:
   std::string &m_string;
};

bool file_verifier::sane(int indent_step, indented_file &parsed_result) const
{
   bool rc;

   rc = dgd::test::parse(m_content, parsed_result);
   if(!rc) {
      BOOST_TEST_MESSAGE("failed to parse content");
      return false;
   }
   
   BOOST_TEST_MESSAGE("parsed result: ");
   BOOST_TEST_MESSAGE(str(parsed_result));
   
   rc = dgd::test::sane(indent_step, parsed_result);
   if(!rc) {
      BOOST_TEST_MESSAGE("basic file sanity check failed");
      return false;
   }

   return true;
}

bool file_verifier::sane(int indent_step) const
{
   indented_file parsed_result;
   return sane(indent_step, parsed_result);
}


bool file_verifier::sane(int indent_step, indented_line_glob &glob) const
{
   bool rc;

   indented_file parsed_result;
   
   rc = sane(indent_step, parsed_result);
   if(!rc) return false;
      
   rc = dgd::test::sane(parsed_result, glob);
   if(!rc) {
      BOOST_TEST_MESSAGE("globbed file sanity check failed");
      return false;
   }

   return true;
}

std::streamsize file_verifier::load()
{
   namespace io = boost::iostreams;
   
   io::file_source src(m_file.string());
   string_sink dst(m_content);
   return io::copy(src , dst );
}

void sane(const file_verifier& verifier, unsigned int indent_step, indented_line_glob &glob)
{
   BOOST_CHECK( verifier.sane(indent_step, glob) );

   BOOST_TEST_MESSAGE("Glob pos: " << glob.pos());
}

void sane(const file_verifier& verifier, unsigned int indent_step)
{
   BOOST_CHECK( verifier.sane(indent_step) );
}

void sane(const temp_file& log, file_verifier &verifier)
{
   namespace fs = boost::filesystem;
   using namespace boost::phoenix;

   BOOST_CHECK( fs::exists(log.path()) );

   std::streamsize log_file_size = fs::file_size(log.path());
   BOOST_TEST_MESSAGE("Resulting log file size " << log_file_size);
   BOOST_CHECK_GT( log_file_size,  0u );

   verifier.set(log.path());
   std::streamsize log_size = verifier.load();
   BOOST_TEST_MESSAGE("Loaded " << log_size << " bytes of log to memory");
   BOOST_CHECK_GT( log_size, 0u );
   
   BOOST_TEST_MESSAGE("---> Begin Content");
   BOOST_TEST_MESSAGE(verifier.content());
   BOOST_TEST_MESSAGE("---> End Content");
}

void sane(const temp_file& log, unsigned int indent_step, indented_line_glob &glob)
{
   file_verifier verifier;
   
   sane(log, verifier);
   sane(verifier, indent_step, glob);
}

void sane(const temp_file& log, unsigned int indent_step)
{
   file_verifier verifier;
   
   sane(log, verifier);
   sane(verifier, indent_step);
}


} // end of namespace test
} // end of namespace dgd

// 
// Local Variables:
// compile-command: "make dg_file_verifier.obj"
// End:
//


