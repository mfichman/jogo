import os

# Build specification for the Apollo Programming Language ####################

VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(ENV = os.environ)

build_mode = ARGUMENTS.get('mode', 'debug')
stack_size = '8192'
major_version = '0'
minor_version = '2'
revision = '0'
version = major_version + '.' + minor_version + '.' + revision

# OS X-specific build settings ###############################################
if env['PLATFORM'] == 'darwin':
    env.Append(CXXFLAGS = '-DDARWIN')
    env.Append(CFLAGS = '-DDARWIN')
    nasm = 'nasm -dDARWIN -fmacho64 -o $TARGET $SOURCE'

# Linux-specific build settings ##############################################
if env['PLATFORM'] == 'posix':
    env.Append(CXXFLAGS = '-DLINUX')
    env.Append(CFLAGS = '-DLINUX')
    nasm = 'nasm -dLINUX -felf64 -o $TARGET $SOURCE'

# Windows-specific build settings ############################################
if env['PLATFORM'] == 'win32':
    nsis = '"' + os.environ['PROGRAMFILES'] + '\\NSIS\\makensis.exe"\
         /DVERSION='+version + '\
         /V2 /NOCD \
         dist\\win\\Apollo.nsi'

    nasm = 'nasm -DWINDOWS -fwin64 -o $TARGET $SOURCE'
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
    env.Append(CXXFLAGS = '/DVERSION=\\"'+version+'\\"')
    env.Append(CFLAGS = '/DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '/DWINDOWS')
    env.Append(CFLAGS = '/Iruntime')
    dist_path = 'dist\\root'
else:
    bld = Builder(action = nasm, src_suffix = '.asm', suffix = '.o')
    if 'release' == build_mode:
        env.Append(CXXFLAGS = '-O3')
        env.Append(CFLAGS = '-O3')
    else:
        env.Append(CXXFLAGS = '-g')
        env.Append(CFLAGS = '-g')
    env.Append(LDFLAGS = '-lm')
    env.Append(CXXFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CXXFLAGS = '-Wall -Werror -pedantic -ansi')
    env.Append(CXXFLAGS = '-Wno-unused')
    env.Append(CXXFLAGS = '-Wno-sign-compare')
    env.Append(CXXFLAGS = '-DVERSION=\\"'+version+'\\"')
    env.Append(CFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '-Wall -Werror -std=c99 -Iruntime')
    dist_path = 'dist/root/usr/local'

env.Append(BUILDERS = { 'NASM': bld })

# Compiler build #############################################################
compiler_src = env.Glob('build/compiler/*.cpp')
apollo = env.Program('bin/apollo', compiler_src + ['build/drivers/Main.cpp'])
apdoc = env.Program('bin/apdoc', compiler_src+ ['build/drivers/Doc.cpp'])

# Library/runtime build ######################################################
library_src = env.Glob('build/runtime/*/*.c') 
library_src += env.Glob('build/runtime/*.c')
library_src += env.NASM(env.Glob('build/runtime/*/*.asm'))
library_src += env.NASM(env.Glob('build/runtime/*.asm'))
lib = env.StaticLibrary('lib/apollo', library_src)

# Test commands ##############################################################
if 'check' in COMMAND_LINE_TARGETS:
    check = env.Command('check', apollo, 'ruby scripts/test --verbose')
    env.Depends(check, lib)

if 'test' in COMMAND_LINE_TARGETS:
    test = env.Command('test', apollo, 'ruby scripts/test --full --verbose')
    env.Depends(test, lib)

# Distribution ###############################################################
library_headers = env.Glob('runtime/*/*.ap')
library_headers += env.Glob('runtime/*.ap')
library_headers += env.Glob('runtime/*/*.h')
library_headers += env.Glob('runtime/*.h')

binary_files = env.Glob('bin/*')
library_files = env.Glob('lib/*')


pkgmaker = '/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOs/PackageMaker\
    --doc dist/pkg/Apollo.pmdoc\
    --title Apollo\
    --version ' + version + '\
    --out "Apollo ' + version + '.pkg"'

dpkg = 'dpkg -b dist/root apollo-' + version + '.deb'


if 'pkg' in COMMAND_LINE_TARGETS:
    for f in library_headers:
        path = f.path.split(os.path.sep)
        path = os.path.join(*path[1:])
        path = os.path.join(dist_path, 'include/apollo', path)
        copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
    
    for f in binary_files:
        path = os.path.join(dist_path, f.path)
        copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
    
    for f in library_files:
        path = os.path.join(dist_path, f.path)
        copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)


    if env['PLATFORM'] == 'darwin':
        pkg = env.Command('pkg', apollo, pkgmaker)
    
    if env['PLATFORM'] == 'posix':
        copy = env.Command('dist/root/DEBIAN', 'dist/deb/DEBIAN', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        pkg = env.Command('pkg', apollo, dpkg)

    if env['PLATFORM'] == 'win32':
        path = os.path.join(dist_path, 'LICENSE.txt')
        copy = env.Command(path, 'LICENSE.txt', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        path = os.path.join(dist_path, 'README.txt')
        copy = env.Command(path, 'README.md', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        pkg = env.Command('pkg', apollo, nsis)

    env.Depends(pkg, lib)

