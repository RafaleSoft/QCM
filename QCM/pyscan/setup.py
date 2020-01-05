from distutils.core import setup, Extension

setup(name='pyscan',
      version='1.0',
      author="Fabrice FERRAND",
      author_email="fabfer@wanadoo.fr",
      description="Python Extension Package to QCM scan",
      keywords="QCM scan qrcode",
      ext_modules=[Extension('pyscan',
                             ['pyscan.c', 'writepng.c', 'dbgutil.c'],
                             libraries=['qrgen', 'quirc', 'zlib', 'libpng', 'libjpeg', 'scan'],
                             define_macros=[('NDEBUG', None),
                                            ('ZLIB_DLL', None),
                                            ('PNG_USE_DLL', None)]
                             )]
      )
