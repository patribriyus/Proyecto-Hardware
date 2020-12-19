
    .extern main
    .equ STACK, 0x0c7ff000

start: 
    ldr sp,=STACK
    mov fp,#0
    
    mov lr,pc
    ldr pc,=main

End:
    B End

	.end

