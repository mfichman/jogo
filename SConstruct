import os

VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(ENV = os.environ)

build_mode = ARGUMENTS.get('mode', 'debug')
stack_size = '8192'

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
    apollo = 'bin\\apollo.exe'
    bld = Builder(action = nasm, src_suffix = '.asm', suffix = '.obj')
    if 'release' == build_mode:
        env.Append(CXXFLAGS = '/O2')
        env.Append(CFLAGS = '/O2')
    else:
        env.Append(CXXFLAGS = '/MDd /Zi')
        env.Append(CFLAGS = '/MDd /Zi') 
        env.Append(LINKFLAGS = '/DEBUG')
    env.Append(CXXFLAGS = '/DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CXXFLAGS = '/DWINDOWS')
    env.Append(CXXFLAGS = '/EHsc')
    env.Append(CFLAGS = '/DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '/DWINDOWS')
    env.Append(CFLAGS = '/Iruntime')
else:
    apollo = 'bin\apollo'
    bld = Builder(action = nasm, src_suffix = '.asm', suffix = '.o')
    if 'release' == build_mode:
        env.Append(CXXFLAGS = '-O3')
        env.Append(CFLAGS = '-O3')
    else:
        env.Append(CXXFLAGS = '-g')
        env.Append(CFLAGS = '-g')
    env.Append(CXXFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CXXFLAGS = '-Wall -Werror -pedantic -ansi')
    env.Append(CXXFLAGS = '-Wno-unused')
    env.Append(CXXFLAGS = '-Wno-sign-compare')
    env.Append(CFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '-Wall -Werror -std=c99 -Iruntime')

env.Append(BUILDERS = { 'NASM': bld })

compiler_sources = env.Glob('build/compiler/*.cpp')
env.Program('bin/apollo', compiler_sources +  ['build/drivers/Main.cpp'])
env.Program('bin/apdoc', compiler_sources + ['build/drivers/Doc.cpp'])

library_sources = env.Glob('build/runtime/*/*.c') 
library_sources += env.Glob('build/runtime/*.c')
library_sources += env.NASM(env.Glob('build/runtime/*/*.asm'))
library_sources += env.NASM(env.Glob('build/runtime/*.asm'))
lib = env.StaticLibrary('lib/apollo', library_sources)

if 'check' in COMMAND_LINE_TARGETS:
    check = env.Command('check', apollo, 'ruby scripts/test --verbose')
    env.Depends(check, lib)

if 'test' in COMMAND_LINE_TARGETS:
    test = env.Command('test', apollo, 'ruby scripts/test --full --verbose')
    env.Depends(test, lib)
