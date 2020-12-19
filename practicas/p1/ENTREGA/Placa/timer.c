/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "44b.h"
#include "44blib.h"

/*--- variables globales ---*/
int switch_leds = 0;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
// Cuando llega a 0. El timer LANZA LA INTERRPUCIÓN
void timer_ISR(void)
{
	switch_leds = 1; // El main está pendiente de este valor

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}
// -- Se definen valores de los registros del controlador de interrupciones con las igualdades --

void timer_init(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ											--- INTERRPUT MODE REGISTER 	(*(volatile unsigned *)0x1e00008)
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)							--- INTERRUPT CONTROL REGISTER	(*(volatile unsigned *)0x1e00000)
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 		--- INTERRUPT MASK REGISTER		(*(volatile unsigned *)0x1e0000c)
							//(bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)		--- BIT_TIMER0: (0x1<<13) ??????????????????????????

	// &= ELIMINA BITS

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER0 = (unsigned) timer_ISR;

	/* Configura el Timer0 */
	rTCFG0 = 255; // ajusta el preescalado															--- (*(volatile unsigned *)0x1d50000)
	rTCFG1 = 0x0; // selecciona la entrada del mux que proporciona el reloj.						--- (*(volatile unsigned *)0x1d50004)
				  // La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 12800;// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0x09;
}

