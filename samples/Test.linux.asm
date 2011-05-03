section .text
    global main

Main.main:
    ret

main:

    mov eax, 0x4
    mov ebx, 1
    mov ecx, msg
    mov edx, len
    int 0x80

    call Main.main
    
    mov eax, 0x1
    int 0x80

section .data
    msg db "hello, world", 0xa
    len equ $-msg
