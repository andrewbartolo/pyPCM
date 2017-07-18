from distutils.core import setup, Extension

module1 = Extension('pyPCM',
                    sources = ['py_pcm.c'])

setup (name = 'pyPCM',
       version = '1.0',
       description = 'Python hooks for Intel PCM.',
       ext_modules = [module1])
