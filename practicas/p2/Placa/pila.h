#ifndef _PILA_H_
#define _PILA_H_

#include "stdint.h"

enum evento{
	ev_timer0 				= 0xd,
	ev_timer1				= 0xe,
	ev_timer4 				= 0xf,
	p_desconocida 			= 0x5,
	p_boton_none  			= 0xa,
	p_boton_izq   			= 0xb,
    p_boton_der   			= 0xc,
    EST_INICIO				= 0x13,
    EST_REBOTES_PRESION		= 0x14,
    EST_MANTIENE_PULSADO	= 0x15,
    EST_REBOTES_DEPRESION	= 0x16,
};


void push_debug(uint8_t ID_evento, uint32_t auxData);
int hay_eventos_encolados(uint32_t stack_anterior);
uint32_t asignar_stack();
uint32_t leer_evento(uint32_t stack_anterior);

#endif
