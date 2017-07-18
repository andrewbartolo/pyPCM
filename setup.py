from distutils.core import setup, Extension

PCM_DIR = '/file0/bartolo/build-new/pcm'

module1 = Extension('pyPCM',
                    sources = ['py_pcm.cpp'],
                    language = 'c++11',
                    extra_compile_args = ['-std=c++11', '-I', PCM_DIR, '-L', 
                                          PCM_DIR+'/cpucounters.o', PCM_DIR+'/pci.o',
                                          PCM_DIR+'/msr.o', PCM_DIR+'/client_bw.o',
                                          PCM_DIR+'/cpucounters.o'])

setup (name = 'pyPCM',
       version = '1.0',
       description = 'Python hooks for Intel PCM.',
       ext_modules = [module1])
