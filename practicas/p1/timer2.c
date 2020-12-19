/*********************************************************************************************
* Fichero:		timer2.c
* Autor:
* Descrip:		funciones de control del timer2 para medir tiempos
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/*--- variables globales ---*/
int timer2_num_int;
struct timeval t_ini, t_fin;


/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	timer2_num_int++;
	gettimeofday(&t_ini, NULL);
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER0 = (unsigned) timer2_ISR;

	/* Configura el Timer2 */
	rTCFG0 = 255; // ajusta el preescalado
	rTCFG1 = 0x0; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 12800;// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/
	rTCON = 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON = 0x09;
	
	timer2_empezar();
}

void timer2_empezar(void){
	gettimeofday(&t_ini, NULL);
	timer2_num_int = 0;
}

unsigned int timer2_leer(void){
	gettimeofday(&t_fin, NULL);
	return (t_fin.tv_sec*1000000 + t_fin.tv_usec) - (t_ini.tv_sec*1000000 + t_ini.tv_usec);
}

unsigned int timer2_parar(void){
	system("PAUSE");
	return timer2_leer();
}