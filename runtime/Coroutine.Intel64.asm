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

%define RSP_OFFSET 40; Stack pointer
%define STATUS_OFFSET 24
%define CURRENT_OFFSET 48 

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

%ifdef WINDOWS
%define ARG0 rcx
%else
%define ARG0 rdi
%endif

cglobal Coroutine__resume
cglobal Coroutine__exit
cglobal Coroutine__yield
cextern Coroutine__stack
cextern Coroutine__current
cextern CoroutineStatus_NEW
cextern CoroutineStatus_RUNNING
cextern CoroutineStatus_SUSPENDED
cextern CoroutineStatus_DEAD

section .text

Coroutine__resume:
    ; Resume the coroutine passed in via ARG0 by saving the state of the current
    ; coroutine, and loading the other corountine's state.  Then, 'return' to
    ; the caller of the other coroutine's yield() invocation.
    ; **** NOTE: If any of the 'push' instructions below change, then
    ; Coroutine.c must also be modified!!!
    push rbp
    push rbx
    push rdi
    push ARG0
    push r12
    push r13
    push r14
    push r15
    push qword [Coroutine__current]
    push qword [Coroutine__stack]
    push qword [save_rsp]
    mov [save_rsp], rsp
    mov rsp, [ARG0+RSP_OFFSET]
    mov [Coroutine__current], ARG0
    mov rax, [ARG0+CURRENT_OFFSET]
    mov [Coroutine__stack], rax;
    pop r15
    pop r14
    pop r13
    pop r12
    pop ARG0
    pop rdi
    pop rbx
    pop rbp
    ret
    
Coroutine__exit:
    ; This is the same as yield, except it sets the status code to 'DEAD'
    ; because the coroutine is finished.
    mov ARG0, [Coroutine__current]
    mov rax, [CoroutineStatus_DEAD]
    mov qword [ARG0+STATUS_OFFSET], rax
    jmp Coroutine__yield

Coroutine__yield:
    ; Yield to the calling coroutine by saving the state of the current
    ; coroutine, and loading the other coroutine's state.  Then, 'return' to
    ; the calling coroutine's resume() invocation.
    ; **** NOTE: If any of the 'push' instructions below change, then
    ; Coroutine.c must also be modified!!!
    push rbp
    push rbx
    push rdi
    push ARG0
    push r12
    push r13
    push r14
    push r15
    mov ARG0, [Coroutine__current]
    mov rax, [Coroutine__stack]
    mov [ARG0+CURRENT_OFFSET], rax
    mov [ARG0+RSP_OFFSET], rsp
    mov rsp, [save_rsp]
    pop qword [save_rsp]
    pop qword [Coroutine__stack]
    pop qword [Coroutine__current]
    pop r15
    pop r14
    pop r13
    pop r12
    pop ARG0
    pop rdi
    pop rbx
    pop rbp
    ret
    
section .bss

save_rsp resq 1

