from distutils.core import setup, Extension

setup(name='pyscan',
      version='1.0',
      author="Fabrice FERRAND",
      author_email="fabfer@wanadoo.fr",
      description="Python Extension Package to QCM scan",
      keywords="QCM scan qrcode",
      ext_modules=[Extension('pyscan',
                             ['pyscan.c'],
                             include_dirs=['/usr/include',
                                           '../../qrgen',
                                           '../../scan',
                                           'F:\QCM\zlib-1.2.11\include',
                                           'F:\QCM\JpegLib-v9\include',
                                           'F:\VisualStudioProjects\Raptor\AddOns\TiffLib-4.0.10\include',
                                           'F:\QCM\libpng-1.6.36\include'],
                             libraries=['qrgen', 'quirc', 'z', 'png', 'jpeg', 'scan'],
                             library_dirs=['../venv/libs64'],
                             # extra_compile_args=['/Zi'],
                             # extra_link_args=['/PDB:"F:\QCM\QCM\\venv\Lib\site-packages\pyscan.pdb"','/DEBUG'],
                             define_macros=[('NDEBUG', None),
                                            ('ZLIB_DLL', None),
                                            ('PNG_USE_DLL', None),
                                            ('QRGEN_EXPORTS', None),
                                            ('_CRT_SECURE_NO_WARNINGS', None)]
                             )]
      )
