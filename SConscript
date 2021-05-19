#!/usr/bin/env python

import os, re, string, subprocess
Import('env')
#------------------------------------------------------------------------------
# last two components of the path. Ex: /not/this/but/THIS/AND_THIS
#                                      "AND_THIS" is usually "src"
#------------------------------------------------------------------------------
x = subprocess.call('scripts/build_config',shell=True)

stntuple_env = env.Clone()
stntuple_env['CPPPATH' ].append('-I'+os.environ['BUILD_BASE']+'/include');
stntuple_env['CXXFLAGS'].append('-I'+os.environ['BUILD_BASE']+'/include');
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------
# Import('mu2e_helper')
from stntuple_helper import *

stntuple_env.Append(BUILDERS = {'StntupleCodegen'  : stntuple_codegen })
stntuple_env.Append(BUILDERS = {'StntupleRootCint' : stntuple_rootcint})

Export('stntuple_env')
Export('stntuple_helper')
