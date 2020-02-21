from distutils.core import setup, Extension

setup(name='pyscan',
      version='1.0',
      author="Fabrice FERRAND",
      author_email="fabfer@wanadoo.fr",
      description="Python Extension Package to QCM scan",
      keywords="QCM scan qrcode",
      ext_modules=[Extension('pyscan',
                             ['pyscan.c', 'writepng.c', 'dbgutil.c'],
                             include_dirs=['/usr/include',
                                           'F:\QCM\zlib-1.2.11\include',
                                           'F:\VisualStudioProjects\Raptor\AddOns\JpegLib-v9\include',
                                           'F:\VisualStudioProjects\Raptor\AddOns\TiffLib-4.0.10\include',
                                           'F:\QCM\libpng-1.6.36\include'],
                             libraries=['qrgen', 'quirc', 'zlib', 'libpng', 'libjpeg', 'scan'],
                             library_dirs=['F:/QCM/QCM/venv/libs64'],
                             extra_compile_args=['/Zi'],
                             extra_link_args=['/PDB:"F:\QCM\QCM\\venv\Lib\site-packages\pyscan.pdb"','/DEBUG'],
                             define_macros=[('NDEBUG', None),
                                            ('ZLIB_DLL', None),
                                            ('PNG_USE_DLL', None),
                                            ('_CRT_SECURE_NO_WARNINGS', None)]
                             )]
      )
