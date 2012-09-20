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
import pickle
import atexit

dgscons_path=os.path.join('.', 'dgscons')
sys.path.append(dgscons_path)

import dgscons.version
import dgscons.build_status

env = DefaultEnvironment(tools = ['mingw'])

compile_mode = '-g'
variant = 'Debug'

system_root = os.environ['SystemRoot']
system32_root = os.path.join(system_root, 'System32')

mingw_root = 'd:/s/mingw'

boost_root = 'd:/s/boost-1.47'

env.Append(CCFLAGS = compile_mode, 
           CXXFLAGS = compile_mode, 
           LINKFLAGS = compile_mode)

env['BOOSTDIR'] = boost_root
env['BOOSTINCDIR'] =  os.path.join(boost_root, 'include/boost-1_47')
env['BOOST_LIB_SUFFIX'] = '-mgw45-mt-d-1_47'

env['ENV'] = {
    'PATH': [ system32_root, os.path.join(mingw_root,'bin') ],
    'SystemRoot': system_root,
    'ComSpec': os.environ['ComSpec'],
    'TEMP':  os.environ['TEMP'],
    'TMP':  os.environ['TMP'],
}

version = dgscons.version.version()
version.incr()

print "Version: " + str(version)
#print "Env: " + str(env.Dump())

Export('env', 'version', 'variant')

SConscript( ['dgd/SConscript', 
             'test/SConscript'] )

atexit.register(dgscons.build_status.handle_build_atexit, version)
