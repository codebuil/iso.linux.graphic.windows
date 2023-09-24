org 100h
mov ax,13h
int 10h
mov ax,cs
mov es,ax
mov ax,3
mov bx,cmds
int 22h
mov ax,0
int 21h
int 20
ret
ret
cmds db "hello.c32",0
files db "hello.txt",0
endf db 0
