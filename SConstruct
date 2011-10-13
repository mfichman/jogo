import os

VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(ENV = os.environ)
env.Append(YACCFLAGS = '--defines=build/compiler/Grammar.hpp')
env.Append(YACCFLAGS = '--report=all')

if env['PLATFORM'] == 'darwin':
    env.Append(CXXFLAGS = '-DDARWIN')
    env.Append(CFLAGS = '-DDARWIN')
    nasm = 'nasm -fmacho64 -o $TARGET $SOURCE'

if env['PLATFORM'] == 'posix':
    env.Append(CXXFLAGS = '-DLINUX')
    env.Append(CFLAGS = '-DLINUX')
    nasm = 'nasm -felf64 -o $TARGET $SOURCE'

if env['PLATFORM'] == 'win32':
    nasm = 'nasm -fwin64 -o $TARGET $SOURCE'
    env.Append(CXXFLAGS = '/DWINDOWS')
    env.Append(CXXFLAGS = '/MDd')
    env.Append(CXXFLAGS = '/Zi')
    env.Append(LINKFLAGS = '/DEBUG')
    env.Append(CFLAGS = '/DWINDOWS')
    env.Append(CFLAGS = '/Iruntime')
    env.Append(CFLAGS = '/MDd')
    env.Append(CFLAGS = '/DCOROUTINE_STACK_SIZE=8192')
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


bld = Builder(action = nasm, src_suffix = '.nasm', suffix = '.obj')
env.Append(BUILDERS = { 'NASM': bld })

compiler_sources = env.Glob('build/compiler/*.cpp')
env.Program('bin/apollo', compiler_sources +  ['build/drivers/Main.cpp'])
env.Program('bin/apdoc', compiler_sources + ['build/drivers/Doc.cpp'])

library_sources = env.Glob('build/runtime/*/*.c') 
library_sources += env.Glob('build/runtime/*.c')
library_sources += env.NASM(
    'build/runtime/Coroutine.Intel64.o',
    'runtime/Coroutine.Intel64.asm')

lib = env.StaticLibrary('lib/apollo', library_sources)

if 'check' in COMMAND_LINE_TARGETS:
    check = env.Command('check', 'bin/apollo', 'scripts/test --verbose')
    env.Depends(check, lib)

if 'test' in COMMAND_LINE_TARGETS:
    test = env.Command('test', 'bin/apollo', 'scripts/test --full --verbose')
    env.Depends(test, lib)
