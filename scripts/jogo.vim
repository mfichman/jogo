" Vim syntax file
" Language: Apollo
" Maintainer: Matt Fichman <matt.fichman@gmail.com>
" Info: Yeeehaw!
" URL: http://stanford.edu/~mfichman
" Release Coordinator: Matt Fichman <matt.fichman@gmail.com>

syn keyword apKeyword import public private native while
syn keyword apKeyword if for let ret xor and or in else until not
syn keyword apKeyword func break var const weak is yield match with 
syn keyword apKeyword macro class interface embedded

syn keyword apTodo contained TODO FIXME XXX

syn cluster apCommentGroup contains=apTodo
syn region apComment start="#" end="$" contains=apTodo
"syn region apRegex start="/" end="/"

syn region apString matchgroup=apStringDelim start=+"+ skip=+\\\\\|\\"+ end=+"+ contains=apInterpolation
syn region apString matchgroup=apStringDelim start=+'+ skip=+\\\\\|\\'+ end=+'+
syn match apType "'\@<=c"
syn match apType "'\@<=b"

syn region apInterpolation matchgroup=apStringDelim start="#{" end="}" contained contains=ALL


syn keyword apConstant true false nil self eof


syn match apFunction "^ *[@]\?[a-z0-9_][a-z0-9_]*[=!?]\?\(([^(){}]*)[^(){}]*{\)\@="
"syn match apFunction "\([A-Z][A-Za-z0-9_]*::\)\@<=[@=!?a-z0-9_]*(\@="
"syn match apFunction "[a-z][A-Za-z0-9_]*[=!?]\=\((.*{\)\@="
"syn match apFunction "[a-z][A-Za-z0-9_]*[=!?]\=\([^\)]*func\)\@="
syn match apString ":\@<!:[a-z][a-z0-9_]*"
"syn match apFunction "\.\@<=[a-z][A-Za-z0-9_]*[=!?]\=\>"
syn match apType "[A-Z][A-Za-z0-9]*"
syn match apConstant "[A-Z][A-Z0-9_]*[a-z]\@!"
syn match apString "[a-zA-Z0-9_]\@<!\/.*\/"


syn keyword apOperator + - / * <> == != ^ ?

syn match apNumber "\<[0-9][0-9]*\>"
syn match apNumber "\<0x[0-9A-Fa-f][0-9A-Fa-f]*\>"

hi def link apKeyword Keyword
hi def link apTodo Todo
hi def link apComment Comment
hi def link apString String
hi def link apConstant Constant
hi def link apNumber Number
hi def link apFunction Function
hi def link apType Type
hi def link apOperator Operator
hi def link apStringDelim Delimiter
hi def link apSymbol Constant
hi def link apRegex String

let b:current_syntax = "apollo"

