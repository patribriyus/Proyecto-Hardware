/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/*--- variables globales ---*/
// int switch_leds = 0;
volatile int timer2_num_int = 0;

/*--- RESUMEN DE REGISTROS USADOS EN TIMER ---*/
/*
 * TFG0: Configuraci�n de prescalado y dead-zone
 * 	- Bits[0:7]   Prescalado de Timers 0 y 1
 *  - Bits[8:15]  Prescalado de Timers 2 y 3
 *  - Bits[16:23] Prescalado de Timers r y 5
 *  - Bits[24:31] Dead Zone
 *
 * TCFG1: Configuraador del divisor de frecuencia
 * - 4 bits por cada Multiplexor. 5 MUX EN TOTAL
 * - Por cada MUX:
 * 		- 0000: Divisor 1/2
 * 		- 0001: Divisor 1/4
 * 		- 0010: Divisor 1/8
 * 		- 0011: Divisor 1/16
 * 		- 01XX: Divisor 1/32
 * 		*Consultar tabla
 *
 * TCON: Controlador del timer
 *  - Bit 0: Start/stop Timer 0
 *  - Bit 1: Manual update Timer 0
 *  - Bit 2: Inverter on/off
 *  - Bit 3: Auto-reload on/off
 *  - Bit 4: Dead Zone on/off
 *  ... Se repite este ciclo por cada timer
 *
 * TCNTBx: N�mero de ciclos inicial que carga TCNTx
 * TCMPBx: N�mero de ciclos inicial que carga TCMPx
 * TCNTOx: Valor actual de TCNTx
 */


/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/

// Cuando llega a 0. El timer LANZA LA INTERRPUCI�N
void timer2_ISR(void)
{
	timer2_num_int++; // Sumamos 1 la variable timer
	switch_leds = 1; // El main est� pendiente de este valor

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/

	// ! Indica FIN de la ISR al controlador de interrupciones
	// Enmascara la interrupci�n TIMER2
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 11 que correponde al Timer2

	// |= -> Significa que va a mantener el estado de rI_ISPC pero con el nuevo bit indicado. Es decir, hace una OR
	// entre lo  que ya tiene y el bit entrante.
}

// -- Se definen valores de los registros del controlador de interrupciones con las igualdades --

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */													
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ											--- INTERRPUT MODE REGISTER 	(*(volatile unsigned *)0x1e00008)
	rINTCON = 0x1; // Habilita interrupciones vectorizadas y la linea IRQ (FIQ no)					--- INTERRUPT CONTROL REGISTER	(*(volatile unsigned *)0x1e00000)
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 		--- INTERRUPT MASK REGISTER		(*(volatile unsigned *)0x1e0000c)
							//(bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)		--- BIT_TIMER0: (0x1<<13) ??????????????????????????

	// &= ELIMINA BITS -> Es un AND sobre rINTMSK y BIT_TIMER2

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER2 = (unsigned) timer2_ISR; // Le decimos que es timer2_ISR qui�n debe tratar la interrupci�n

	/* Configura el Timer2 */
	rTCFG0 |= 0x0; // ajusta el preescalado. 255 hace referencia a los primeros 8 bits, es decir, temporizadores 0 y 1															--- (*(volatile unsigned *)0x1d50000)
	rTCFG1 |= 0x0; // selecciona la entrada del mux que proporciona el reloj.						--- (*(volatile unsigned *)0x1d50004)
				  // La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = 0;// valor de comparaci�n
	/* establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x6000; // Realmente el inverter no deber�a hacer falta y podr�a ser 0x2000
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0x9000; // Timer start = 1
				     // Manual update = 0
				     // Inverter = 0
				     // Timer auto-reload = 1
	rTCON &= ~(0x2000); // Manual update = 0
}


void timer2_empezar() {
	timer2_num_int = 0;
	  // La 00 corresponde a un divisor de 1/2.
	/* establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON |= 0x6000; // Realmente el inverter no deber�a hacer falta y podr�a ser 0x2000
		/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON |= 0x9000; // Timer start = 1
	rTCON &= ~(0x2000);
	    // Manual update = 0
  	   	// Inverter = 0
	    // Timer auto-reload = 1

}


unsigned int timer2_leer() {
	// TCNTB2 = COUNT BUFFER REGISTER
	// TCNTO2 = REGISTRO DE OBSERVACI�N

	// M�ltiplos

	return (timer2_num_int * (rTCNTB2) + (rTCNTB2 - rTCNTO2)) / 32;
}

unsigned int timer2_parar() {
	rINTMSK |= BIT_TIMER2; // Enmascaramos para evitar que lleguen interrupciones mientras leemos TCNB2 y TCNTO2
	rTCON &= 0xFFF; // Reset.

	return timer2_leer();
}
