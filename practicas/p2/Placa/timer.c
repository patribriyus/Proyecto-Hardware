/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include "pila.h"

/*--- variables globales ---*/

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
// Cuando llega a 0. El timer LANZA LA INTERRPUCI�N
void timer_ISR(void) {
	push_debug(ev_timer0, 0);

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
	timer_stop();
}
// -- Se definen valores de los registros del controlador de interrupciones con las igualdades --

void timer_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ											--- INTERRPUT MODE REGISTER 	(*(volatile unsigned *)0x1e00008)
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)							--- INTERRUPT CONTROL REGISTER	(*(volatile unsigned *)0x1e00000)
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 		--- INTERRUPT MASK REGISTER		(*(volatile unsigned *)0x1e0000c)
							//(bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)		--- BIT_TIMER0: (0x1<<13) ??????????????????????????
	// &= ELIMINA BITS

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer_ISR;

	/* Configura el Timer0 */
	rTCFG0 |= (0xE); // PREESCALADO: 14 ajusta el preescalado															--- (*(volatile unsigned *)0x1d50000)
	rTCFG1 |= (0x0); // selecciona la entrada del mux que proporciona el reloj.						--- (*(volatile unsigned *)0x1d50004)
				  	  // DIVISOR 2: La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 64000;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 0;// valor de comparaci�n

}

void timer_stop() {
	rINTMSK |= BIT_TIMER0;
	rTCON &= ~(0x1);
}

void timer_empezar() {
	rINTMSK &= ~(BIT_TIMER0);
	rTCON |= 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0x9;
	rTCON &= ~(0x2);
}

