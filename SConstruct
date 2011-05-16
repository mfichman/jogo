VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)



env = Environment(CPPPATH = ['build/compiler'])
env.Append(YACCFLAGS = '--defines=build/compiler/Grammar.hpp')
env.Append(YACCFLAGS = '--report=all')
env['AS'] = 'nasm'
env['RANLIB'] = 'echo'
if env['PLATFORM'] == "darwin":
    env['AR'] = 'libtool'
    env['ARFLAGS'] = '-static -o'
    env.Append(CXXFLAGS = '-DDARWIN')
    env.Append(ASFLAGS = '-fmacho64')
if env['PLATFORM'] == "posix":
    env.Append(CXXFLAGS = '-DLINUX')
    env.Append(ASFLAGS = '-felf64')
#    env.Append(ARFLAGS = '-X64')
if env['PLATFORM'] == "windows":
    env.Append(CXXFLAGS = '-DWINDOWS')
env.Append(CXXFLAGS = '-g')
env.Append(CXXFLAGS = '-Wall -Werror -pedantic -ansi')
env.Append(CXXFLAGS = '-Wno-unused')
env.Append(CXXFLAGS = '-Wno-sign-compare')
env.Append(CFLAGS = '-g')
env.Append(CFLAGS = '-Wall -Werror -pedantic -std=c99 -m64')

compiler_sources = env.Glob('build/compiler/*.cpp')
compiler_sources += ['build/compiler/Grammar.yy', 'build/compiler/Lexer.ll'] 
env.Program('bin/apollo', compiler_sources +  ['build/drivers/Main.cpp'])
env.Program('bin/test', compiler_sources + ['build/drivers/Test.cpp'])


library_sources = env.Glob('build/runtime/*.c') 
library_sources += env.Glob('build/runtime/*.asm')
env.StaticLibrary('lib/apollo', library_sources)
#env.Program('bin/asmtest', 'samples/Test.linux.asm')

if 'check' in COMMAND_LINE_TARGETS:
    env.Command('check', 'bin/test', 'scripts/test')

if 'test' in COMMAND_LINE_TARGETS:
    env.Command('test', 'bin/test', 'scripts/test --full --verbose')
