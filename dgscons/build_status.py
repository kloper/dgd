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

import sys

from . import version

def bf_to_str(bf):
    """Convert an element of GetBuildFailures() to a string
    in a useful way."""
    import SCons.Errors
    if bf is None: # unknown targets product None in list
        return '(unknown tgt)'
    elif isinstance(bf, SCons.Errors.StopError):
        return str(bf)
    elif bf.node:
        return str(bf.node) + ': ' + bf.errstr
    elif bf.filename:
        return bf.filename + ': ' + bf.errstr
    return 'unknown failure: ' + bf.errstr

def build_status():
    """Convert the build status to a 2-tuple, (status, msg)."""
    from SCons.Script import GetBuildFailures
    bf = GetBuildFailures()
    if bf:
        # bf is normally a list of build failures; if an element is None,
        # it's because of a target that scons doesn't know anything about.
        status = 'failed'
        failures_message = "\n".join(["Failed building %s" % bf_to_str(x)
                                      for x in bf if x is not None])
    else:
        # if bf is None, the build completed successfully.
        status = 'ok'
        failures_message = ''
    return (status, failures_message)

def handle_build_atexit(version):
    """Display the build status.  Called by atexit.
    Here you could do all kinds of complicated things."""
    (status, failures_message) = build_status()
    if status == 'failed':
        print "scons: Build Failed" 
    elif status == 'ok':
        try:
            version.save()
        except:
            print "scons: unable to save version: ", sys.exc_info()
        print "scons: Build succeeded. New version: {}".format(version)
    if failures_message:
        print "scons: {}".format(failures_message)
