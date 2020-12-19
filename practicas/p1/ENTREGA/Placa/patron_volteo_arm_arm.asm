.global patron_volteo_arm_arm

.equ PATRON_ENCONTRADO, 1
.equ NO_HAY_PATRON, 0
.equ DIM, 7
.equ CASILLA_VACIA, 0

patron_volteo_arm_arm:
.arm

	push {r4-r9,fp,lr}

	@ Parametros a preguntar
	@ R0 = TABLERO
	@ R1 = @*Longitud
	@ R2 = FA
	@ R3 = CA
	@ R4 = fp + 4 = SF
	@ R5 = fp + 8 = SC
	@ R6 = fp + 12 = COLOR
	@ R7 = Longitud
	@ R8 = POSICION_VALIDA
	@ R9 = CASILLA (ficha)

	@ ! Se podría hacer Load Múltiple!

	ldrsb r4, [sp, #32]	@ r4 = SF
	ldrsb r5, [sp, #36]	@ r5 = SC
	ldrb r6, [sp, #40]	@ r6 = COLOR

	ldrb r7, [r1]		@ R7 = Longitud (tambien podriamos usar -> mov r7, #0 ya que la funcion
						@ actualizar_tablero siempre invoca a patron_volteo con longitud = 0 y ahorrar un acceso a memoria)

while:
	add r2, r2, r4 		@ r6 = FA = FA + SF
	add r3, r3, r5 		@ r8 = CA = CA + SC

	@ FICHA VALIDA
	if_1:
		cmp r2,#DIM			@ f < DIM
		bge else_1

		cmp r2,#0			@ f >= 0
		blt else_1

		cmp r3,#DIM			@ c < DIM
		bge else_1

		cmp	r3,#0			@ c >= 0
		blt else_1

		add r9,r3,r2,LSL#3			@ r9 = filas * 2^3 + columnas
		ldrb r9,[r0,r9]				@ r9 = CASILLA (ficha)

		cmp	r9,#CASILLA_VACIA		@ tablero[f][c] != CASILLA_VACIA
		movne r8,#1
		bne end_f_valida

	else_1:
		mov r8,#0
		mov r9,#CASILLA_VACIA
	@ END_FICHA_VALIDA

end_f_valida:
	cmp r8, #1		@ posicion_valida == 1
	bne if_2

	cmp r9, r6		@ casilla != color
	addne r7, r7, #1	@ longitud++
	bne while

if_2:
	cmp r8, #1		@ posicion_valida == 1
	bne else_2

	cmp r9, r6		@ casilla == color
	bne else_2

	cmp r7, #0		@ *longitud >0
	movgt r0, #PATRON_ENCONTRADO
	bgt fin

else_2:
	mov r0, #NO_HAY_PATRON
	
fin:
	strb r7,[r1] @ actualizamos el valor de longitud

	pop {r4-r9,fp,lr}
	bx lr
