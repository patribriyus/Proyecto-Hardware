
/*--- ficheros de cabecera ---*/
#include "pila.h"
#include "timer2.h"
#include "reversi8.h"

#define STACK_INIT 0x0c7ff600;							// Al principio apunta al comienzo de las pilas
														// de los distintos modos de ejecución y va creciendo hacia arriba
#define STACK_END 0x0c7ff400;		// Dejamos 256B de espacio para nuestra pila


/*	------ PILA ------
 * 32b:   TIEMPO
 * 32b: ID | aux_Data
 * -------------------
 *
 * 32 bits para tiempo
 * 8 bits para ID_Evento
 * 24 bits para aux_Data
 *
 */

static volatile uint32_t* stack_pointer = STACK_INIT;    // Puntero de pila.
//void push_debug(void) __attribute__((interrupt("IRQ"))); // Debería hacer falta?

/*--- variables globales ELIMINAR!
// static uint32_t* pos;						// Puntero utilizado internamente para guardar datos en la pila

/*--- codigo de las funciones ---*/
void reset_pila()
{
	stack_pointer = STACK_INIT;
}

// ID_evento = bits 0-7
// auxData = bits 0-23
void push_debug(uint8_t ID_evento, uint32_t auxData)
{
	uint32_t tiempo = timer2_leer(); // tiempo en microsegundos cuando ocurre

	if (stack_pointer == 0x0c7ff400) {
		reset_pila();
	}

	stack_pointer--; // Dejamos hueco para poner ID_evento y AuxData

	/*
    * Desplazamos ID_evento 24 bits para que ocupe los 8 bits más significativos
	* y concatenamos con los 24 bits de auxData para formar la palabra de 32 bits.
	* De tal forma: [ ID (8 bits) + auxData (24 bits) ] = 32 bits.
	*/

	*stack_pointer = (ID_evento << 24) | auxData;
	stack_pointer--; // Dejamos hueco para poner el tiempo (32 bits)
	*stack_pointer = tiempo;

}

int hay_eventos_encolados(uint32_t stack_anterior) {
	if (stack_pointer == (stack_anterior + 4)) { // +4 para alinear con Stack Pointer
		return 0;
	}

	return 1;
}

uint32_t leer_evento(uint32_t stack_anterior) {
	if (stack_anterior == 0x0c7ff404) { // 0x0c7ff504 = Stack End + 4
		stack_anterior = STACK_INIT;
	}

	return stack_anterior;
}

uint32_t asignar_stack() {
	return stack_pointer;
}


