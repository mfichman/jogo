; Copyright (c) 2010 Matt Fichman
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.


default rel

%define RSP_OFFSET 32; Stack pointer

; OS X prefixes all symbol names with '_', so to interoperate with C, we need
; to add '_' prefixes to all symbol names.  This macro takes care of the
; required name mangling.
%ifidn __OUTPUT_FORMAT__,macho64
%macro  cglobal 1 
global  _%1 
%define %1 _%1 
%endmacro 
%macro  cextern 1 
extern  _%1 
%define %1 _%1 
%endmacro
%else
%define cglobal global
%define cextern extern
%endif

; Windows and Linux/OS X have different calling conventions, so function
; arguments will be in different registers.
%ifdef WINDOWS
%define ARG0 rcx
%define ARG1 rdx
%else
%define ARG0 rdi
%define ARG1 rsi
%endif

cglobal Coroutine__swap

; Pointer to the current top-of-stack.  This is used to determine space
; remaining on the stack, so that a new stack segment can be allocated if
; necessary.  See Coroutine__grow_stack().
cextern Coroutine__stack 

; Pointer to the current coroutine.  Used by Coroutine__resume() to set the
; 'caller' of the coroutine that is being resumed, and by Coroutine__yield() to
; switch from the current coroutine to the caller.
cextern Coroutine__current

section .text
Coroutine__swap: ; (from, to)
    ; Resume the coroutine passed in via ARG0 by saving the state of the current
    ; coroutine, and loading the other corountine's state.  Then, 'return' to
    ; the caller of the other coroutine's yield() invocation.
    ; **** NOTE: If any of the 'push' instructions below change, then
    ; Coroutine.c must also be modified!!!
    mov [Coroutine__current], ARG1
    ; Set the 'current coroutine' equal to ARG1

    push qword [Coroutine__stack] ; Save the top-of-stack pointer
    push ARG0
    push rbp
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    mov [ARG0+RSP_OFFSET], rsp ; Save the sp for 'from'
    mov rsp, [ARG1+RSP_OFFSET] ; Restore the sp for 'to'
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp
    pop ARG0
    pop qword [Coroutine__stack] ; Restore the top-of-stack pointer
    ret
