section .text
global _main
extern _write

_main:
    %line 1 BinaryTree.ap
    push rbp
    %line 2
    mov rbp, rsp
    %line 3

    mov rdi, 1
    %line 4
    mov rsi, msg
    %line 4
    mov rdx, len
    %line 4
    call _write

    %line 4
    mov rsp, rbp
    %line 4
    pop rbp
    %line 4
    ret
    
    ;mov rdi, 0
    ;mov rax, 0x2000001
    ;syscall

section .data
    msg db "hello, world", 0xa, 0x0
    len equ $-msg
    dq 88
