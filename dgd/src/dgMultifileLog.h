// -*- c++ -*-
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
// dgMultifileLog.h -- multifile log funnel
//

#ifndef _dgMultifileLog_h_
#define _dgMultifileLog_h_

#include <iostream>
#include <fstream>
#include <string>

#include "dgFunnel.h"

namespace DGD {

class multifile_log: public funnel {
   public:
      class Split_criteria {
	 public:
	    virtual bool operator () ( unsigned long volume,
				       unsigned long line, 
				       unsigned long column,
				       unsigned long bytes ) const = 0;
      };

      class Dont_split: public Split_criteria {
	 public:
	    bool operator () (  unsigned long volume,
				unsigned long line, 
			       unsigned long column,
			       unsigned long bytes ) const { return false; };
      };

      class Split_by_size: public Split_criteria  {
	 public:
	    Split_by_size() : m_vol_size(1024*1024*5) {} // 5M per volume
	    Split_by_size( unsigned long vol_size ) : m_vol_size(vol_size) {}

	    bool operator () ( unsigned long volume,
			       unsigned long line, 
			       unsigned long column,
			       unsigned long bytes ) const { 
	       return ( bytes >= volume * m_vol_size ); 
	    }
	 private:
	    unsigned long m_vol_size;
      };

      class Split_by_lines: public Split_criteria  {
	 public:
	    Split_by_lines() : m_vol_size(60000) {} // ~5M per volume
	    Split_by_lines( unsigned long vol_size ) : m_vol_size(vol_size) {}
	    Split_by_lines( const Split_by_lines& peer ) : 
	       m_vol_size( peer.m_vol_size ) {}

	    bool operator () ( unsigned long volume,
			       unsigned long line, 
			       unsigned long column,
			       unsigned long bytes ) const { 
	       return ( line >= volume * m_vol_size ); 
	    }
	 private:
	    unsigned long m_vol_size;
      };

   public:
      static Dont_split dont_split;
      static Split_by_size split_by_size;
      static Split_by_lines split_by_lines;
	    
   public:
      multifile_log( const char* name, 
		     Split_criteria* criteria = &split_by_size );
      ~multifile_log();

   private:
      void operator () ( unsigned long line, 
			 unsigned long column,
			 unsigned long bytes );
      std::string real_name() const;
      
   private:
      std::ofstream m_file;
      std::string   m_name;
      unsigned int  m_part_count;
      Split_criteria* m_split;
};


}; // end of namespace DGD

#endif /* _dgMultifileLog_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgMultifileLog.obj"
 * End:
 */


