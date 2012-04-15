// -*- c++ -*-
// 
// Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// 
// This file is part of dgd library (http://dgd.sf.net).
// 
// dgd.h -- common includes for dgd
// 

#ifndef _dgd_h_
#define _dgd_h_

#include <string>
#include <sstream>
#include <fstream>
#include <exception>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/thread/tss.hpp>
#include <boost/thread/thread.hpp>

#include <boost/program_options.hpp>

/*
 * This dummy template method is required to satisfy gcc and make it accept
 * filtering_stream::component<T>() method. For some reason without this trick
 * gcc complains when trying to use it.
 */
template<typename T>
T* component(int n) {
   throw std::exception("dummy component");
}

#include <dg_filter.h>
#include <dg_exception.h>
#include <dg_journal_filter.h>
#include <dg_wrapper_filter.h>
#include <dg_channel.h>
#include <dg_manip.h>
#include <dg_controller.h>
#include <dg_scope.h>
#include <dg_expand.h>

#if defined(QT_CORE_LIB)
#include <QtCore/QString>
#include <dg_qt4.h>
#endif

#endif /* _dgd_h_ */

/* 
 * Local Variables:
 * compile-command: "make dgd.obj"
 * End:
 */


