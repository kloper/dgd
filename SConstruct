# -*- mode:python -*-
# 
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# 
# This file is part of dgd library (http://dgd.sf.net).
#
# SConstruct -- main construction file for DGD
#

import os
import sys
import atexit

dgscons_path=os.path.join('..', 'dgscons')
sys.path.append(dgscons_path)

import dgscons
import dgscons.version
import dgscons.build_status

dgscons_tools_path = os.path.join(dgscons_path, 'tools')
boost = Tool('boost', [ dgscons_tools_path ])
hardlink = Tool('hardlink', [ dgscons_tools_path ])

env = dgscons.setup_environment(tools = ['textfile', boost, hardlink])

version = dgscons.version.version()
version.incr()

#print "Version: " + str(version)
#print "Env: " + str(env.Dump())

Export('env', 'version')

SConscript( ['dgd/SConscript', 
             'test/SConscript',
             'samples/SConscript'] )

atexit.register(dgscons.build_status.handle_build_atexit, version)
