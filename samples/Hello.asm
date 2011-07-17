section .text
global _main
extern _write

_main:
    push rbp
    mov rbp, rsp

    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    call _write

    mov rsp, rbp
    pop rbp
    ret
    
    ;mov rdi, 0
    ;mov rax, 0x2000001
    ;syscall

section .data
    msg db "hello, world", 0xa, 0x0
    len equ $-msg
    dq 88
