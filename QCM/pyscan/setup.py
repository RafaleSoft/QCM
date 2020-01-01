from distutils.core import setup, Extension

setup(name='PyScan', version='1.0',
      ext_modules=[Extension('PyScan', ['pyscan.c'])])
