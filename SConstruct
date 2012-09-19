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
import pickle

env = DefaultEnvironment(tools = ['mingw'])

compile_mode = '-g'
variant = 'Debug'

system_root = os.environ['SystemRoot']
system32_root = os.path.join(system_root, 'System32')

mingw_root = 'c:/personal/mingw'

boost_root = 'c:/personal/boost-1.47'

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

#  version = { 'build': 3, 'compile': 330, 'major': 2, 
#              'stable': 0, 'timestamp': 1347458080 }
# >>> with open('.version', 'wb') as vfile:
# ...     pickle.dump(version, vfile)
# ...

with open('.version', 'rb') as vfile:
    version = pickle.load(vfile)

#print "Version: " + str(version)
#print "Env: " + str(env.Dump())

Export('env', 'version', 'variant')

SConscript( ['dgd/SConscript', 
             'test/SConscript'] )
