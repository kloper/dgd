//
// $Id$
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// This file is part of Depression Glass library.
//
// Copyright (c) 2002. Dimitry Kloper <dimka@cs.technion.ac.il> . 
//
// dgMultifileLog.cpp -- implementation for dgMultifileLog.h
//

#include <boost/lexical_cast.hpp>

#include "dgMultifileLog.h"

namespace DGD {

multifile_log::Dont_split     multifile_log::dont_split;
multifile_log::Split_by_size  multifile_log::split_by_size;
multifile_log::Split_by_lines multifile_log::split_by_lines;

multifile_log::multifile_log( const char* name, 
			      Split_criteria* criteria ) :
   funnel( m_file ),
   m_name( name ),
   m_part_count(0),
   m_split( criteria )
{
   m_file.open( real_name().c_str() );
   m_part_count++;
   setstate( m_file.rdstate() );
}

multifile_log::~multifile_log() {
   m_file.flush();
   m_file.close();
}

void multifile_log::operator () ( unsigned long line, 
				  unsigned long column,
				  unsigned long bytes ) {
   if( m_split != NULL && 
       m_split->operator()( m_part_count, line, column, bytes ) ) {
      m_file.flush();
      m_file.close();
      
      m_file.open( real_name().c_str() );
      m_part_count++;
      setstate( m_file.rdstate() );
      for( unsigned int i = 0; i < column; i++ )
	 m_file.put(' ');
   }
}

std::string multifile_log::real_name() const {
   std::string::size_type pos = m_name.find_last_of( '.' );
   if( pos == std::string::npos )
      return m_name;
   
   std::string real_name = m_name;
   real_name.insert( pos, boost::lexical_cast<std::string>( m_part_count ) );
   
   return real_name;
}

}; // end of namespace DGD


// 
// Local Variables:
// compile-command: "make dgMultifileLog.obj"
// End:
//


