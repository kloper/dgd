# -*- mode:python -*-
# 
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# 
# This file is part of dgd library (http://dgd.sf.net).
#
# dgscons/build_status.py -- routines for handling build results
# see http://www.scons.org/doc/2.0.1/HTML/scons-user/x2045.html
#

import os
import sys
import string
import re

from SCons.Script import AddOption
from SCons.Script import GetOption
from SCons.Script import ARGUMENTS
from SCons.Defaults import DefaultEnvironment

class DGSConsException(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

AddOption('--compiler', 
          dest='compiler',
          default='mingw',
          type='string',
          nargs=1,
          action='store',
          help='compiler environment to use')

AddOption('--variant', 
          dest='variant',
          default='Debug',
          type='string',
          nargs=1,
          action='store',
          help='variant to compile')

def setup_environment(tools):
    env = None
    if sys.platform.find('win') == 0:
        system_root = os.environ['SystemRoot']
        comspec = os.environ['ComSpec']
        env = {
            'PATH': [ os.path.dirname(comspec) ],
            'SystemRoot': os.environ['SystemRoot'],
            'ComSpec': comspec,
            'TEMP':  os.environ['TEMP'],
            'TMP':  os.environ['TMP']
            }
    else:
        raise DGSConsException("Platform {} is not supported".
                               format(os.platform))

    compiler = GetOption('compiler')
    if compiler == 'mingw':
        mingwbin = ARGUMENTS.get('mingwbin')
        if not mingwbin or not os.path.isdir(mingwbin):
            raise DGSConsException("Unable to determine "
                                   "Mingw compiler location. "
                                   "Provided 'mingwbin' value: {}".
                                   format(mingwbin))
        env['PATH'].append(mingwbin)
        tools.append('mingw')
    else:
        raise DGSConsException("Compiler type {} is not supported".
                               format(compiler))
    
    compile_mode = ''
    variant = GetOption('variant')

    if variant == 'Debug':
        compile_mode = '-g'
    elif variant == 'Release':
        compile_mode = '-O2'
    else:
        raise DGSConsException("Variant {} is not supported".format(variant))
    
    return DefaultEnvironment(ENV = env, 
                              tools = tools,
                              VARIANT = variant,
                              CCFLAGS = compile_mode, 
                              CXXFLAGS = compile_mode, 
                              LINKFLAGS = compile_mode)

