# -*- mode:python -*-
# 
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2002-2012
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
# 
# This file is part of dgd library (http://dgd.sf.net).
#
# dgscons/version.py -- serialized product version 
#

"""
Handle build version 
"""

import os
import time
import pickle
import string

class version:
    vfile_name = ".version"
    
    def __init__(self, file_name = vfile_name):
        if os.path.isdir(file_name):
            file_name += vfile_name

        self.file_name = file_name

        if not os.path.isfile(file_name):
            self.version = {
                'compile': 0,
                'stable':  0,
                'build':   0,
                'major':   0,
                'timestamp': time.time()
                }
        else:
            with open(file_name, 'rb') as vfile:
                self.version = pickle.load(vfile)

    def __str__(self):
        return "{}.{}.{}.{} ({})".format(
            self.version['stable'],
            self.version['major'],
            self.version['build'],
            self.version['compile'],
            time.ctime(self.version['timestamp']))

    def incr(self, what = 'compile'):
        self.version[what] += 1

    def save(self):
        self.version['timestamp'] = time.time()
        with open(self.file_name, 'wb') as vfile:
            pickle.dump(self.version, vfile)
