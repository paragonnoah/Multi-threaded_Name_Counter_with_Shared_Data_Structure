global _start

section .text:
_start:
        mov eax,0
        mov al,0


        add al,0x34
        add ah,0x12
        add ax,0x2

        mov bx,0x1236
        sub ax,bx

        mov ecx,0
        inc ecx
        dec ecx