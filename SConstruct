import platform
import os
import sys
import shutil

# Build specification for the Jogo Programming Language ####################
(system, _, release, version, machine, proc) = platform.uname()
print(' '.join((system, release, version, machine, proc)))

if 'help' in COMMAND_LINE_TARGETS:
    print
    print("Jogo build targets")
    print
    print("release   Create a new release and upload to github.com")
    print("doc       Generate wiki documentation")
    print("pkg       Create a new installer package")
    print("check     Run tests with memory checker disabled")
    print("test      Run full test suite")
    print
    sys.exit(0);

VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/debugger', 'debugger', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)
VariantDir('build/runtime', 'runtime', duplicate=0)

build_dir = os.path.join('build', 'runtime')
env = Environment(CPPPATH = ['build/compiler', 'build/debugger'])
env.Append(ENV = os.environ)
env.Append(JGFLAGS = '-v -m -i runtime --build-dir ' + build_dir)
env.Append(JGFLAGS = ' --no-default-libs -g Intel64 ')

build_mode = ARGUMENTS.get('mode', 'debug')
#stack_size = '32768'
stack_size = '1048576' # x8 = 8 MB
major_version = '0'
minor_version = '4'
patch = '0'
version = major_version + '.' + minor_version + '.' + patch
branch = os.popen('git rev-parse --abbrev-ref HEAD').read().strip()
revision = os.popen('git rev-parse HEAD').read().strip()

if 'release' == build_mode:
    env.Append(JGFLAGS = '--optimize')

# OS X-specific build settings ###############################################
if env['PLATFORM'] == 'darwin':
    env.Append(CXXFLAGS = '-DDARWIN')
    env.Append(CFLAGS = '-DDARWIN')
    nasm = '/usr/local/bin/nasm -dDARWIN -fmacho64 -o $TARGET $SOURCE'

# Linux-specific build settings ##############################################
if env['PLATFORM'] == 'posix':
    env.Append(CXXFLAGS = '-DLINUX')
    env.Append(CFLAGS = '-DLINUX -m64 -lm')
    nasm = 'nasm -dLINUX -felf64 -o $TARGET $SOURCE'

# Windows-specific build settings ############################################
if env['PLATFORM'] == 'win32':
    nsis = '"' + os.environ['PROGRAMFILES'] + '\\NSIS\\makensis.exe"\
         /DVERSION='+version+'\
         /DREVISION='+revision+'\
         /DBRANCH='+branch+'\
         /V2 /NOCD \
         dist\\win\\Jogo.nsi'

    nasm = 'nasm -DWINDOWS -fwin64 -o $TARGET $SOURCE'
    nasm_bld = Builder(action = nasm, src_suffix = '.asm', suffix = '.obj')
    if 'release' == build_mode:
        env.Append(CXXFLAGS = '/MT /Zi /O2')
        env.Append(CFLAGS = '/MT /Zi /O2')
    else:
        env.Append(CXXFLAGS = '/MT /Zi')
        env.Append(CFLAGS = '/MT /Zi') 
        env.Append(LINKFLAGS = '/DEBUG')
    env.Append(CXXFLAGS = '/DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CXXFLAGS = '/DWINDOWS')
    env.Append(CXXFLAGS = '/EHsc')
    env.Append(CXXFLAGS = '/DVERSION=\\"'+version+'\\"')
    env.Append(CXXFLAGS = '/DREVISION=\\"'+revision+'\\"')
    env.Append(CXXFLAGS = '/DBRANCH=\\"'+branch+'\\"')
    env.Append(CFLAGS = '/DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '/DWINDOWS')
    env.Append(CFLAGS = '/Iruntime')
    dist_path = 'dist\\root'
else:
    nasm_bld = Builder(action = nasm, src_suffix = '.asm', suffix = '.o')
    if 'release' == build_mode:
        env.Append(CXXFLAGS = '-O3')
        env.Append(CFLAGS = '-O3')
    else:
        env.Append(CXXFLAGS = '-g')
        env.Append(CFLAGS = '-g')
    env.Append(LDFLAGS = '-lm')
    env.Append(CXXFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CXXFLAGS = '-Wall -Werror -ansi')
    env.Append(CXXFLAGS = '-Wno-unused')
    env.Append(CXXFLAGS = '-Wno-sign-compare')
    env.Append(CXXFLAGS = '-DVERSION=\\"'+version+'\\"')
    env.Append(CXXFLAGS = '-DREVISION=\\"'+revision+'\\"')
    env.Append(CXXFLAGS = '-DBRANCH=\\"'+branch+'\\"')
    env.Append(CFLAGS = '-DCOROUTINE_STACK_SIZE='+stack_size)
    env.Append(CFLAGS = '-Wall -Werror -Iruntime')
    if env['PLATFORM'] == 'darwin':
        dist_path = 'dist/root/usr/local'
    else:
        dist_path = 'dist/root/usr'

env.Append(BUILDERS = { 'NASM': nasm_bld })

# Compiler build #############################################################
compiler_src = env.Glob('build/compiler/*.cpp')
jogo_cmd = os.path.join('bin', 'jogo')
jgdoc_cmd = os.path.join('bin', 'jgdoc')
jgmake_cmd = os.path.join('bin', 'jgmake')
jgfix_cmd = os.path.join('bin', 'jgfix')
jgi_cmd = os.path.join('bin', 'jgi')
jogo = env.Program(jogo_cmd, compiler_src + ['build/drivers/Main.cpp'])
jgdoc = env.Program(jgdoc_cmd, compiler_src + ['build/drivers/Doc.cpp'])
jgmake = env.Program(jgmake_cmd, compiler_src + ['build/drivers/Make.cpp'])
jgfix = env.Program(jgfix_cmd, compiler_src + ['build/drivers/Fix.cpp'])
compiler = env.StaticLibrary('lib/jogoc', compiler_src)

# Debugger build #############################################################

debugger_src = env.Glob('build/debugger/*.cpp')
debugger = env.StaticLibrary('lib/jgdebug', debugger_src)
jgdebug_cmd = os.path.join('bin', 'jgdebug')
jgdebug = env.Program(jgdebug_cmd, compiler_src + debugger_src + ['build/drivers/Debugger.cpp'])

# Library/runtime build ######################################################
library_src = ' '.join([
    "Array",
    "Boot::Boot",
    "Collection",
    "Chan",
    "Coroutine",
    "File",
    "Func",
    "Hash",
    "Http",
    "Io",
    "Json",
    "Math",
    "Object",
    "Os",
    "Pair",
    "Primitives",
    "Queue",
    "Regex",
    "Socket",
    "String",
    "Uri",
    "Utf8",
])

coroutine = env.NASM('build/runtime/Coroutine.Intel64.asm')

jogo_lib = os.path.join('lib', 'Jogo')
lib = env.Command('jglib', jogo, '%s $JGFLAGS -o %s %s' % (jogo_cmd, jogo_lib, library_src))
env.Depends(lib, jogo)
env.Depends(lib, coroutine)

# Test commands ##############################################################
if 'check' in COMMAND_LINE_TARGETS:
    check = env.Command('check', jogo, 'python scripts/test --verbose')
    env.Depends(check, lib)
    env.Depends(check, compiler)

if 'test' in COMMAND_LINE_TARGETS:
    test = env.Command('test', jogo, 'python scripts/test --full --verbose')
    env.Depends(test, lib)
    env.Depends(test, compiler)

# Distribution ###############################################################
library_headers = env.Glob('runtime/*/*.jg')
library_headers += env.Glob('runtime/*.jg')
library_headers += env.Glob('runtime/*/*.h')
library_headers += env.Glob('runtime/*.h')

binary_files = env.Glob('bin/*')
library_files = env.Glob('lib/*')


pkgmaker = '/Applications/PackageMaker.app/Contents/MacOs/PackageMaker\
    --doc dist/pkg/Jogo.pmdoc\
    --title Jogo\
    --version ' + version + '\
    --out "jogo-' + version + '.pkg"'

dpkg = 'dpkg -b dist/root jogo-' + version + '.deb'

#rpmbuild = 'rpmbuild\
#    --define="version ' + version + '"\
#    --define="_topdir /dist/root

if 'doc' in COMMAND_LINE_TARGETS:
    doc = env.Command('doc', jgdoc, 'bin/jgdoc $JGFLAGS -o wiki ' + library_src)
    env.Depends(doc, jogo)
    env.Depends(doc, coroutine)
    env.Depends(doc, lib)  


if 'pkg' in COMMAND_LINE_TARGETS:
    shutil.rmtree(dist_path)
    for f in library_headers:
        path = f.path.split(os.path.sep)
        path = os.path.join(*path[1:])
        path = os.path.join(dist_path, 'include', 'jogo', path)

        copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
    
    for f in binary_files:
        if os.path.join("bin", "test") not in f.path and ".ilk" not in f.path:
            path = os.path.join(dist_path, f.path)
            copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
            env.Depends('pkg', copy)
    
    for f in library_files:
        if 'jogoc' not in f.path:
            path = os.path.join(dist_path, f.path)
            copy = env.Command(path, f, Copy('$TARGET', '$SOURCE'))
            env.Depends('pkg', copy)


    if env['PLATFORM'] == 'darwin':
        pkg = env.Command('pkg', jogo, pkgmaker)
    
    if env['PLATFORM'] == 'posix':
        copy = env.Command('dist/root/DEBIAN', 'dist/deb/DEBIAN', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        pkg = env.Command('pkg', jogo, dpkg)

    if env['PLATFORM'] == 'win32':
        path = os.path.join(dist_path, 'LICENSE.txt')
        copy = env.Command(path, 'LICENSE.txt', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        path = os.path.join(dist_path, 'README.txt')
        copy = env.Command(path, 'README.md', Copy('$TARGET', '$SOURCE'))
        env.Depends('pkg', copy)
        pkg = env.Command('pkg', jogo, nsis)

    env.Depends(pkg, lib)

if 'release' in COMMAND_LINE_TARGETS:
    release = env.Command('release', jogo, 'python scripts/release '+version)
    env.Depends('pkg', release)


