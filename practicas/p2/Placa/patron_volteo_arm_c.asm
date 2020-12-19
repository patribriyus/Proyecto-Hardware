.global patron_volteo_arm_c
.extern ficha_valida

posicion_valida: .space 4	@ Reservar espacio en la pila

.equ PATRON_ENCONTRADO, 1
.equ NO_HAY_PATRON, 0

patron_volteo_arm_c:
.arm

	push {r4-r10,fp,lr}

	@ Parámetros recibidos de C
	@ R0 = TABLERO
	@ R1 = @*Longitud
	@ R2 = FA
	@ R3 = CA
	@ sp + 36 = SF
	@ sp + 40 = SC
	@ sp + 44 = COLOR

	@ Paso de parametros a ficha_valida (r0 - r3 los valores cambian)
	@ R0 = Tablero
	@ R1 = FA
	@ R2 = CA
	@ R3 = DIR_POS_VAL
	@ --------------------
	@ R4 = Longitud
	@ R5 = DIR_POS_VAL
	@ R6 = FA
	@ R7 = COLOR
	@ R8 = CA
	@ R9 = @*Longitud
	@ R10 = TABLERO

	mov r9, r1			@ R9 = @*Longitud
	ldrb r4, [r9]		@ R4 = Longitud (tambien podriamos usar -> mov r4, #0 ya que la funcion
						@ actualizar_tablero siempre invoca a patron_volteo con longitud = 0 y ahorrar un acceso)
	ldr r5, =posicion_valida @ DIR_POS_VAL
	ldrb r7, [sp, #44]	@ r7 = COLOR
	mov r6, r2			@ r6 guarda el valor de FA actualizado
	mov r8, r3			@ r8 guarda el valor de CA actualizado
	mov r10, r0			@ r10 = TABLERO

while:
	ldrsb r1, [sp, #36]	@ r1 = SF
	add r6, r6, r1 		@ r6 = FA = FA + SF
	mov r1, r6			@ r1 = FA

	ldrsb r2, [sp, #40]	@ r2 = SC
	add r8, r8, r2 		@ r8 = CA = CA + SC
	mov r2, r8			@ r2 = CA

	mov r3,r5			@ r3 = @POSICION_VALIDA

	bl ficha_valida

	@ R0 = CASILLA (ficha_valida ha machacado el valor de TABLERO)
	ldrb r1,[r5]	@ r1 = POSICION_VALIDA

	cmp r1, #1		@ posicion_valida == 1
	bne if

	cmp r0, r7		@ casilla != color
	addne r4, r4, #1	@ longitud++
	movne r0, r10 	@ hay que actualizar el valor de r0 con el de TABLERO

	bne while

if:
	cmp r1, #1		@ posicion_valida == 1
	bne else

	cmp r0, r7		@ casilla == color
	bne else

	cmp r4, #0		@ *longitud >0
	movgt r0, #PATRON_ENCONTRADO
	bgt fin

else:
	mov r0, #NO_HAY_PATRON
	
fin:
	strb r4,[r9] @ actualizamos el valor de longitud

	pop {r4-r10,fp,lr}
	bx lr
