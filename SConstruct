env = Environment(
          CPPPATH=['./include/'],
          LIBPATH=['./lib/'],
          TARGET_ARCH='x86_64')
#env.Replace(CCFLAGS = [])  # fix for /nologo error on windows

#env.Tool('crossmingw', toolpath = ['scons-tools'])
env.Program('HelloWorld', 'HelloWorld.cpp')