VariantDir('build/compiler', 'compiler', duplicate=0)
VariantDir('build/drivers', 'drivers', duplicate=0)


SetOption('num_jobs', 2)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(YACCFLAGS = ['--defines=build/compiler/Grammar.hpp'])
env['AS'] = 'nasm'
env.Append(ASFLAGS = '-felf')
env.Append(CXXFLAGS = '-g')
env.Append(CXXFLAGS = '-Wall -Werror -pedantic')
env.Append(CXXFLAGS = '-Wno-unused')

compiler_sources = []
compiler_sources += env.Glob('build/compiler/*.cpp')
compiler_sources += ['build/compiler/Grammar.yy', 'build/compiler/Lexer.ll'] 

env.Program('bin/apollo', compiler_sources +  ['build/drivers/Main.cpp'])
env.Program('bin/test', compiler_sources + ['build/drivers/Test.cpp'])
env.Program('bin/asmtest', 'samples/Test.linux.asm')

env.Command('test', 'bin/test', 'scripts/test')
