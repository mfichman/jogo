section .text
    global _main

Main.main:
    ret

_main:
    push len
    push msg
    push 1
    mov eax, 0x4
    sub esp, 4
    int 0x80
    add esp, 16

    call Main.main
    
    push dword 0
    mov eax, 0x1
    sub esp, 4
    int 0x80

section .data
    msg db "hello, world\n"
    len equ $-msg

