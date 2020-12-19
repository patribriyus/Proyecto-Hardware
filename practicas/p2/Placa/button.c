/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "pila.h"
#include "botones_antirebotes.h"

/*--- variables globales del módulo ---*/
/* int_count la utilizamos para sacar un número por el 8led.
  Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces hay rebotes! */
static unsigned int int_count = 0;
// enum estado_button {button_none, button_iz, button_dr}; // Eliminar después
/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
	rINTMSK   |= BIT_EINT4567; // Enmascaramos interrupciones
	rI_ISPC   |= BIT_EINT4567;
	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;
	switch (which_int)
	{
		case 4:
			int_count++; // incrementamos el contador
			push_debug(p_boton_izq, 0x0);
			break;
		case 8:
			int_count--; // decrementamos el contador
			push_debug(p_boton_der, 0x0);
			break;
		default:
			break;
	}

	// }
	//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (módulo 16)

	/* Finalizar ISR */

}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void button_iniciar(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    = 0x20000;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMSK    &= ~(BIT_EINT4567); // habilitamos interrupcion linea eint4567 en vector de mascaras

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	//rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}


void button_resetear() {
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND.
	rINTMSK &= ~(BIT_EINT4567); // Habilitamos interrupciones para la lína de EINT4567
}


enum estado_button button_estado(void) { // Se puede devolver un ENUM?
	// Los pulsadores están conectados a los pines 6 y 7 del puerto G
	// Sabremos si están presionados o no si:
	// - Valor de bit 6 o 7 = 1 (Vdd) -> NO está presionado
	// - Valor de bit 6 o 7 = 0 (GND 0) -> está presionado
	char derecho_pulsado = ~rPDATG & (1 << 7); // bit 7: botón derecho
	char izquierdo_pulsado = ~rPDATG & (1 << 6); // bit 8: botón izquierdo

	if (derecho_pulsado) return button_dr;
	else if (izquierdo_pulsado) return button_iz;
	return button_none;

}


void button_ev_pulsacion(enum estado_button boton) {
	// Enmascarar interrupciones (ya se hace nada más comienza la RSI de Button)
	timer4_empezar();
	boton_pulsado(boton); // Iniciaría máquina de estados

}

void button_ev_tick0 () {
	botones_antirebotes_gestion();
}

