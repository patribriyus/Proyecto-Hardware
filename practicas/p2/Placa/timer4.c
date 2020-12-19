/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer4.h"
#include "44b.h"
#include "44blib.h"
#include "pila.h"

/*--- variables globales ---*/

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer4_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
// Cuando llega a 0. El timer LANZA LA INTERRPUCIÓN
void timer4_ISR(void)
{
	timer4_stop();
	unsigned int tiempo = timer2_leer();
	push_debug(ev_timer4, 0);
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER4; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}
// -- Se definen valores de los registros del controlador de interrupciones con las igualdades --

void timer4_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ											--- INTERRPUT MODE REGISTER 	(*(volatile unsigned *)0x1e00008)
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)							--- INTERRUPT CONTROL REGISTER	(*(volatile unsigned *)0x1e00000)
	rINTMSK &= ~(BIT_TIMER4); // habilitamos en vector de mascaras de interrupcion el Timer0 		--- INTERRUPT MASK REGISTER		(*(volatile unsigned *)0x1e0000c)
							//(bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)		--- BIT_TIMER0: (0x1<<13) ??????????????????????????

	// &= ELIMINA BITS

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER4 = (unsigned) timer4_ISR;

	/* Configura el Timer4 */
	rTCFG0 |= (0x300000); // PREESCALADO: ajusta el preescalado															--- (*(volatile unsigned *)0x1d50000)
	rTCFG1 |= (0x0); // selecciona la entrada del mux que proporciona el reloj.						--- (*(volatile unsigned *)0x1d50004)
				  	  // DIVISOR 2: La 00 corresponde a un divisor de 1/2.
	rTCNTB4 = 65306;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB4 = 0;// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/

	/* iniciar timer (bit 0) con auto-reload (bit 3)*/

}

void timer4_stop() {
	rINTMSK |= BIT_TIMER4;
	rTCON &= ~(0x100000); // MODIFICAR ESTO
}

void timer4_empezar() {
	rINTMSK &= ~(BIT_TIMER4);
	rTCON |= 0x200000;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0x900000;
	rTCON &= ~(0x200000);
}

