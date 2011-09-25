" Vim syntax file
" Language: Apollo
" Maintainer: Matt Fichman <matt.fichman@gmail.com>
" Info: Yeeehaw!
" URL: http://stanford.edu/~mfichman
" Release Coordinator: Matt Fichman <matt.fichman@gmail.com>

syn keyword apKeyword import case when public private static native while
syn keyword apKeyword if for let ret return xor and or in else until pass not
syn keyword apKeyword func break def fork immutable weak

syn keyword apKeyword class struct interface yield 

syn keyword apTodo contained TODO FIXME XXX

syn cluster apCommentGroup contains=apTodo
syn region apComment start="#" end="$" contains=apTodo

syn region apString matchgroup=apStringDelim start=+"+ skip=+\\\\\|\\"+ end=+"+ contains=apInterpolation
syn region apString matchgroup=apStringDelim start=+'+ skip=+\\\\\|\\'+ end=+'+
syn match apString "\'(.\|..)\'c"

syn region apInterpolation matchgroup=apStringDelim start="#{" end="}" contained contains=ALL


syn keyword apConstant true false nil self eof


"syn keyword apFunction @init @destroy @add @subtract @divide @mulitply
"syn keyword apFunction @index @modulus @bitand @bitor @bitxor @equal
"syn keyword apFunction @less @shift @unshift @power @complement @increment
"syn keyword apFunction @decrement @dispatch  @compare

syn match apFunction "[@][a-z][a-z]*"

syn match apType "[A-Z][A-Za-z0-9]*"
syn match apFunction "[a-z][A-Za-z0-9_]*[=!?]\=(\@="
syn match apString ":\@<!:[a-z][a-z0-9_]*"
"syn match apFunction "\.\@<=[a-z][A-Za-z0-9_]*[=!?]\=\>"


syn keyword apOperator + - / * <> == != ^ ?

syn match apNumber "\<[0-9][0-9]*\>"

hi def link apKeyword Keyword
hi def link apTodo Todo
hi def link apComment Comment
hi def link apString String
hi def link apConstant Constant
hi def link apNumber Number
hi def link apFunction Function
hi def link apType Structure 
hi def link apOperator Operator
hi def link apStringDelim Delimiter

let b:current_syntax = "apollo"

