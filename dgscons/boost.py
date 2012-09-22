# -*- mode:python -*-
# 
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# 
# This file is part of dgd library (http://dgd.sf.net).
#
# dgscons/boost.py -- boost configuration Tool for SCons 
#

import os
import sys
import string

from dgscons import DGSConsException
from SCons.Script import ARGUMENTS

def find():
    boost_include = ARGUMENTS.get('boostinc')
    boost_lib = ARGUMENTS.get('boostlib')
    boost_lib_suffix = ARGUMENTS.get('boostlibsuf', '')
    return (boost_include, boost_lib, boost_lib_suffix)

def generate(env):
    (boost_include, boost_lib, boost_lib_suffix) = find()
    if not boost_include:
        raise DGSConsException("boostinc not defined")
    if not boost_lib:
        raise DGSConsException("boostlib not defined")
    env['BOOSTLIBDIR'] = boost_lib
    env['BOOSTINCDIR'] = boost_include
    env['BOOST_LIB_SUFFIX'] = boost_lib_suffix

def exists(env):
    (boost_include, boost_lib, boost_lib_suffix) = find()
    if not boost_include:
        return False
    if not boost_lib:
        return False
    return True




