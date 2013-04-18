#!/usr/bin/python
# In case you were wondering, this follows
# NPM's funny coding style. Learn more at:
# https://npmjs.org/doc/coding-style.html

from distutils.core import setup, Extension
import sys, os

additional_include_dirs = []
additional_library_dirs = []

# Try to retrieve OpenNI envvars:
niLib = os.getenv('OPENNI2_REDIST')
niInclude = os.getenv('OPENNI2_INCLUDE')

# Exit if not found
if not (niLib and niInclude):
  sys.stderr.write('''
Sorry, OpenNI was not detected.
Make sure you have it loaded by running:

  source "<path to OpenNI2 SDK>/OpenNIDevEnvironment"

Then try running me again.
'''[1:])
  sys.exit(1)

# Utilities
buildVersion = lambda v: '.'.join(str(a) for a in v[:3]) + v[3]

# Make sure the libraries are found
path = os.getenv('LD_RUN_PATH')
if not path: path = ''
else: path += ':'
os.putenv('LD_RUN_PATH', path+niLib)


################
##   MODULE   ##
## DEFINITION ##
################

version = [0,0,1,'-pre']

module = Extension('openni2'
 , sources = [ 'wrapper.cc' ]

 , define_macros = [ ('PYOPENNI2_MAJOR_VERSION', version[0])
                   , ('PYOPENNI2_MINOR_VERSION', version[1])
                   , ('PYOPENNI2_PATCH_VERSION', version[2])
                   ]

 , include_dirs = [niInclude] +additional_include_dirs
 , library_dirs = [niLib]     +additional_library_dirs
 , libraries = ['OpenNI2', 'boost_python']
 )

setup( name = 'PyOpenNI2'
     , version = buildVersion(version)
     , description = 'Bindings for the OpenNI library.'
     , url = 'https://github.com/jmendeth/PyOpenNI2'
     , ext_modules = [module] )
