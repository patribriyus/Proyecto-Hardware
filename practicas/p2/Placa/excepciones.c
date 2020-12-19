/*********************************************************************************************
* Fichero:		excepciones.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- Ficheros de cabecera ---*/

#include <stdint.h>
#include "44b.h"
#include "44blib.h"

#define INIT 0x0
#define SWI 0x13
#define ABT 0x17
#define UND 0x1B

#define SWI_8LED 1
#define ABT_8LED 2
#define UND_8LED 3
#define BLANK_8LED 16

#define DELAY_MS 50000

// VARIABLES GLOBALES
static volatile uint32_t interrupcion; // Volatile!!
static volatile uint32_t direccion_excepcion; // Dirección de la instrucción que causa la excepción. Volatile!!

/* !¿Esto hace falta?!
void tratamiento_exc(void) __attribute__((interrupt("SWI")));
void tratamiento_exc(void) __attribute__((interrupt("DABORT"))); // SOLO FIQ, IRQ
void tratamiento_exc(void) __attribute__((interrupt("UNDEF")));

*/

void tratamiento_exc(void);

void definir_rutinas(void) {
    pISR_UNDEF 	= (unsigned)tratamiento_exc; // Unsigned!!??
    pISR_SWI 	= (unsigned)tratamiento_exc;
    pISR_DABORT = (unsigned)tratamiento_exc;
}

void tratamiento_exc(void) {
	int n_led;
    __asm__ volatile("mrs %0, cpsr\n" // Comprobar
    				 "and %0, %0, #0x1F"
                     : "+r"(interrupcion)); // R: La variable reside en un registro
    										// +: See lee y modifica
    __asm__ volatile("mov %0, lr" : "=r"(direccion_excepcion)); // = Sobreescribe el valor

    // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0311d/I30195.html
    if (interrupcion == SWI) {
    	direccion_excepcion -= 4;
    	n_led = SWI_8LED;
    } else if (interrupcion == ABT) {
    	direccion_excepcion -= 8;
    	n_led = ABT_8LED;
    } else if (interrupcion == UND) {
    	direccion_excepcion -= 4;
    	n_led = UND_8LED;
    } else { // Debug
    	 while(1);
    }

    while (1)
        {
            D8Led_symbol(n_led);
            Delay(DELAY_MS);
            D8Led_symbol(BLANK_8LED);
            Delay(DELAY_MS);
        }
}


/*
 *
 *
 * while (1)
    {
        D8Led_symbol(numero_8led);
        Delay(DELAY_8LED_US);
        D8Led_symbol(BLANK_8LED);
        Delay(DELAY_8LED_US);
    }
 *
 *
 *
 */

