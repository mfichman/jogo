Apollo Compiler
===============

Apollo is a compiler for an experimental programming language.

Building
--------

1. Check out the repository
2. Install [bison](http://www.gnu.org/software/bison), version > 2.4 and [flex](http://flex.sourceforge.net), version > 2.5
4. Install [scons](http://www.scons.org)
4. Type "scons"

Testing
-------

1. Install [ruby](http://www.ruby-lang.org)
2. Install [valgrind](http://www.valgrind.org)
3. Install [nasm](http://www.nasm.us/pub/nasm/releasebuilds/2.09.10/)
4. Run "scons check" or "scons test"

Compiling a Simple Program
--------------------------

Right now, the compiler has barely any implemented features, but it can run a "hello world" program, and perhaps a few other programs.  Don't be surprised if the compiler bails out while compiling a program that you write.  To run the included hello world program, Bootstrap.ap, try these commands:

1. Install prerequisites (see "Building" and "Testing" above)
1. bin/apollo -p tests -p runtime Bootstrap
2. nasm -fmacho64 out.asm
3. gcc out.o -lapollo -Llib
4. ./a.out