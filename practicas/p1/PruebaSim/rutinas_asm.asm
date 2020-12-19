	.global Intercambiar

Intercambiar:
	mov ip,sp
	stmdb sp!,{fp,ip,lr,pc}
    sub fp,ip,#4

	add r1, r0, r1, lsl #3     @ r1 <- &(T[i])
	add r2, r0, r2, lsl #3     @ r2 <- &(T[j])
	ldrb r3, [r1]              @ r3 <- T[i].prioridad
	ldrb ip, [r2]              @ ip <- T[j].prioridad
	strb r3, [r2]              @ T[j].prioridad <- r3
	strb ip, [r1]              @ T[i].prioridad <- ip
	ldr r3,[r1,#4]             @ r3 <- T[i].id
    ldr ip,[r2,#4]             @ ip <- T[j].id
    str r3,[r2,#4]             @ T[j].id <- r3
    str ip,[r1,#4]             @ T[i].id <- ip

	ldmdb fp,{fp,sp,pc}

	.global SeleccionarPID

SeleccionarPID:
	mov ip,sp
	stmdb sp!,{fp,ip,lr,pc}
	sub fp,ip,#4

	mov ip,#0
LOOP1:
	ldrb lr,[r0,ip,lsl #2]
	cmp lr, r3
	cmplt ip, r1
	addlt ip,ip,#1
	blt LOOP1

	sub lr,r1,ip
	str lr,[r2]
	
	cmp ip, r1
	moveq r0, #0
	addne r0, r0, ip, lsl #2

	ldmdb fp,{fp,sp,pc}

	.extern PosMinPrioridad
	.extern Intercambiar
	.global OrdenaPorPrioridad

OrdenaPorPrioridad:
	mov ip,sp
	stmdb sp!,{r4,fp,ip,lr,pc}
	sub fp,ip,#4
	sub sp, sp, #8

	str r0,[fp,#-16]
	str r1,[fp,#-20]

	mov r4,#0
LOOP2:
	ldr r2,[fp,#-20]
	cmp r4,r2
	bge B1
	
	ldr r0, [fp,#-16]
	mov r1, r4
	bl PosMinPrioridad
	mov r2, r0
	mov r1, r4
	ldr r0, [fp,#-16]
	bl Intercambiar
	add r4, r4, #1
	b LOOP2

B1:
	ldmdb fp,{r4,fp,sp,pc}
	
.end


