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
%define STATUS_OFFSET 24

section .text

global _Coroutine__resume
_Coroutine__resume:
    ; Resume the coroutine passed in via rdi by saving the state of the current
    ; coroutine, and loading the other corountine's state.  Then, 'return' to
    ; the caller of the other coroutine's yield() invocation.
    push rbp
    push qword [current_coroutine]
    push qword [save_rsp]
    mov [save_rsp], rsp
    mov rsp, [rdi+RSP_OFFSET]
    mov [current_coroutine], rdi
    pop rbp
    ret
    
global _Coroutine__exit
_Coroutine__exit:
    ; This is the same as yield, except it sets the status code to '3' because
    ; the coroutine is finished.
    mov rdi, [current_coroutine]
    mov qword [rdi+STATUS_OFFSET], 3
    jmp _Coroutine__yield

global _Coroutine__yield
_Coroutine__yield:
    ; Yield to the calling corountine by saving the state of the current
    ; coroutine, and loading the other coroutine's state.  Then, 'return' to
    ; the calling coroutine's resume() invocation.
    push rbp
    mov rdi, [current_coroutine]
    mov [rdi+RSP_OFFSET], rsp
    mov rsp, [save_rsp]
    pop qword [save_rsp]
    pop qword [current_coroutine]
    pop rbp
    ret
    
section .bss

current_coroutine resq 1
save_rsp resq 1

