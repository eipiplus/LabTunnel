from distutils.core import setup, Extension
setup(name='collectpath', version='1.0',  \
              ext_modules=[Extension('collectpath', ['collectpath.c'])])
