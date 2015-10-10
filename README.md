Jogo Compiler
===============

![Build Status](https://travis-ci.org/mfichman/jogo.svg?branch=master)

Jogo is a compiler for an experimental programming language.

* Quick Tour: https://github.com/mfichman/jogo/wiki/Quick-Tour-of-the-Jogo-Programming-Language
* Wiki: https://github.com/mfichman/jogo/wiki

Downloads
---------

* Debian: [jogo-0.3.0.deb](https://jogo-downloads.s3.amazonaws.com/jogo-0.3.0.deb)
* OS X: [jogo-0.3.0.pkg](https://jogo-downloads.s3.amazonaws.com/jogo-0.3.0.pkg)
* Windows: [jogo-0.3.0.exe](https://jogo-downloads.s3.amazonaws.com/jogo-0.3.0.exe)


Building
--------

1. Check out the repository
2. Install [scons](http://www.scons.org)
3. Install [nasm](http://www.nasm.us)
4. Type "scons"

Testing
-------

1. Install [python](http://www.python.org)
2. Run "scons check"

Using the Compiler
------------------

```
$ jogo MyProgram.jg
$ out
```
