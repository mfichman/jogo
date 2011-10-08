import os

VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(ENV = os.environ)
env.Append(YACCFLAGS = '--defines=build/compiler/Grammar.hpp')
env.Append(YACCFLAGS = '--report=all')
env['AS'] = 'nasm'

if env['PLATFORM'] == 'darwin':
    env.Append(CXXFLAGS = '-DDARWIN')
    env.Append(CFLAGS = '-DDARWIN')
    env.Append(ASFLAGS = '-fmacho64')

if env['PLATFORM'] == 'posix':
    env.Append(CXXFLAGS = '-DLINUX')
    env.Append(CFLAGS = '-DLINUX')
    env.Append(ASFLAGS = '-felf64')

if env['PLATFORM'] == 'win32':
    env.Append(CXXFLAGS = '/DWINDOWS')
    env.Append(CFLAGS = '/DWINDOWS')
    env.Appned(CFLAGS = '/Iruntime')
    env.Append(CFLAGS = '/DCOROUTINE_STACK_SIZE=8192')
    env.Append(CXXFLAGS ='/ID:\Tools\Flex\include')
    env.Append(CXXFLAGS = '/EHsc')
    env.Append(CXXFLAGS = '/DCOROUTINE_STACK_SIZE=8192')
else:
    env.Append(CXXFLAGS = '-g')
    env.Append(CXXFLAGS = '-Wall -Werror -pedantic -ansi')
    env.Append(CXXFLAGS = '-Wno-unused')
    env.Append(CXXFLAGS = '-Wno-sign-compare')
    env.Append(CXXFLAGS = '-DCOROUTINE_STACK_SIZE=8192')
    env.Append(CFLAGS = '-g')
    env.Append(CFLAGS = '-Iruntime')
    env.Append(CFLAGS = '-Wall -Werror -std=c99 -m64')
    env.Append(CFLAGS = '-DCOROUTINE_STACK_SIZE=8192')


#bison = Builder(action='bison --defines $SOURCE -o $TARGET')
#flex = Builder(action='flex -t $SOURCE > $TARGET')
#env.Append(BUILDERS={'Bison':bison, 'Flex':flex})
#parser = env.Bison('build/compiler/Grammar.cpp', 'build/compiler/Grammar.yy')
#lexer = env.Flex('build/compiler/Lexer.cpp', 'build/compiler/Lexer.ll')

parser = 'build/compiler/Grammar.yy'
lexer = 'build/compiler/Lexer.ll'

compiler_sources = env.Glob('build/compiler/*.cpp') + [parser, lexer]
env.Program('bin/apollo', compiler_sources +  ['build/drivers/Main.cpp'])
env.Program('bin/apdoc', compiler_sources + ['build/drivers/Doc.cpp'])


library_sources = env.Glob('build/runtime/*/*.c') 
library_sources += env.Glob('build/runtime/*.c')
library_sources += env.Glob('build/runtime/*/*.asm')
library_sources += env.Glob('build/runtime/*.asm')
lib = env.StaticLibrary('lib/apollo', library_sources)
#env.Program('bin/asmtest', 'samples/Test.linux.asm')

if 'check' in COMMAND_LINE_TARGETS:
    check = env.Command('check', 'bin/apollo', 'scripts/test --verbose')
    env.Depends(check, lib)

if 'test' in COMMAND_LINE_TARGETS:
    test = env.Command('test', 'bin/apollo', 'scripts/test --full --verbose')
    env.Depends(test, lib)
