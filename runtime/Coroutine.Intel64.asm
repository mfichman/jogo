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

%define CONTEXT_OFFSET 32 ; Offset to the 'context' attr in Coroutine
;%define RIP_OFFSET CONTEXT_OFFSET+0 ; Function address/resume address
%define RSP_OFFSET CONTEXT_OFFSET+8 ; Stack pointer
%define RDI_OFFSET CONTEXT_OFFSET+16 ; Function call argument
%define RBX_OFFSET CONTEXT_OFFSET+24
%define RBP_OFFSET CONTEXT_OFFSET+32
%define R12_OFFSET CONTEXT_OFFSET+40
%define R13_OFFSET CONTEXT_OFFSET+48
%define R14_OFFSET CONTEXT_OFFSET+56
%define R15_OFFSET CONTEXT_OFFSET+64
%define STATUS_OFFSET 24

section .text

global _Coroutine__resume
_Coroutine__resume:
    ; Resume the coroutine passed in via rdi by saving the state of the current
    ; coroutine, and loading the other corountine's state.  Then, 'return' to
    ; the caller of the other coroutine's yield() invocation.
    push qword [current_coroutine]
    push qword [save_rsp]
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15

    mov [save_rsp], rsp

    mov rsp, [rdi+RSP_OFFSET]
    mov rbx, [rdi+RBX_OFFSET]
    mov rbp, [rdi+RBP_OFFSET]
    mov r12, [rdi+R12_OFFSET]
    mov r13, [rdi+R13_OFFSET]
    mov r14, [rdi+R14_OFFSET]
    mov r15, [rdi+R15_OFFSET]
    
    mov [current_coroutine], rdi
    ret
    

global _Coroutine__yield
_Coroutine__yield:
    ; Yield to the calling corountine by saving the state of the current
    ; coroutine, and loading the other coroutine's state.  Then, 'return' to
    ; the calling coroutine's resume() invocation.
    mov rdi, [current_coroutine]

    mov [rdi+RSP_OFFSET], rsp
    mov [rdi+RBX_OFFSET], rbx
    mov [rdi+RBP_OFFSET], rbp
    mov [rdi+R12_OFFSET], r12
    mov [rdi+R13_OFFSET], r13
    mov [rdi+R14_OFFSET], r14
    mov [rdi+R15_OFFSET], r15

    mov rsp, [save_rsp]

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    pop qword [save_rsp]
    pop qword [current_coroutine]
    ret
    
section .bss

current_coroutine resq 1
save_rsp resq 1

