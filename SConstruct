env = Environment(
          CPPPATH=['include/'],
          LIBPATH=['lib/'],
          LIBS=['libCreationCore-1.8_s', 'libCreationSplice-1.0.3-beta_s'],
          TARGET_ARCH='x86_64')
#env.Replace(CCFLAGS = [])  # fix for /nologo error on windows

#env.Tool('crossmingw', toolpath = ['scons-tools'])
env.Program('HelloWorld', 'HelloWorld.cpp')