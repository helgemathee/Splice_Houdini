env = Environment(
	CXX_FLAGS = ['-pthread'],
	CPPPATH=['include/'],
	LIBPATH=['lib/'],
	LIBS=['libCreationSplice-1.0.3-beta_s', 'libCreationCore-1.8_s', 'GL', 'libpthread', 'libdl'],
	TARGET_ARCH=['x86_64'])
#env.Replace(CCFLAGS = [])  # fix for /nologo error on windows

#env.Tool('crossmingw', toolpath = ['scons-tools'])
t=env.Program('HelloWorld', 'HelloWorld.cpp')
Default(t)