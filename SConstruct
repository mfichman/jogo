VariantDir('build', 'src', duplicate=0)


SetOption('num_jobs', 2)

env = Environment(CPPPATH = ['build/compiler'])
env.Append(BISONFLAGS = ['--defines=build/compiler/Grammar.hpp'])

compiler_sources = []
compiler_sources += env.Glob('build/compiler/*.cpp')
compiler_sources += ['build/compiler/Grammar.yy', 'build/compiler/Lexer.ll'] 

env.Program('bin/apollo', compiler_sources +  ['build/Main.cpp'])
env.Program('bin/test', compiler_sources + ['build/Test.cpp'])

env.Command('test', 'bin/test', 'scripts/test')
