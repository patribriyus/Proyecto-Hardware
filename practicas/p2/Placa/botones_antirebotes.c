#include "botones_antirebotes.h"
#include "timer.h"
#include "pila.h"
#include "button.h"
#include "stdio.h"

/*--------------------------------------------------------------*/

static unsigned int TRP = 100000;
static unsigned int TIEMPO_ESPERA_COMPROBACION = 30000;
static unsigned int TRD = 100000;

static volatile enum evento estado_actual;
static volatile enum estado_button boton_actual;
static volatile unsigned int tiempo_referencia;
static volatile unsigned int tiempo_actual;

enum estado_button boton_antirrebotes_atendido = button_none;
static enum estado_button atendiendo_boton = button_none;

void botones_antirebotes_init(void)
{
    estado_actual = EST_INICIO;
    boton_actual = button_none;
}

void boton_pulsado(enum estado_button boton_presionado) {
    estado_actual = EST_REBOTES_PRESION;
    boton_actual = boton_presionado;
    atendiendo_boton = boton_presionado;

}

enum estado_button botones_antirebotes_leer(void)
{
    return boton_actual;
}

void botones_antirebotes_gestion() {
    switch (estado_actual) {
    	case EST_INICIO: break;

		case EST_REBOTES_PRESION: // Espera el tiempo TRP
		{
		//	tiempo_actual = tiempo_transcurrido(tiempo_referencia);
		//	if (tiempo_actual > TRP) {
		//	push_debug(EST_MANTIENE_PULSADO, 0);
			estado_actual = EST_MANTIENE_PULSADO;
			timer_empezar();
		//	tiempo_referencia = tiempo_actual;
		//	} else {
		//		push_debug(EST_REBOTES_PRESION, 0);
		//	}
			break;
		}
		case EST_MANTIENE_PULSADO: { // Espera de 30ms pa ver si ha sido depresionado
		//	tiempo_actual = tiempo_transcurrido(tiempo_referencia);
		//	if (tiempo_actual > TIEMPO_ESPERA_COMPROBACION) {
			enum estado_button estado_boton_nuevo = button_estado();
		//		tiempo_referencia = tiempo_actual;

			if (estado_boton_nuevo != boton_actual) {
				boton_actual = estado_boton_nuevo;
				estado_actual = EST_REBOTES_DEPRESION;
				timer4_empezar();
				//push_debug(EST_REBOTES_DEPRESION, 0);
			} else {
				timer_empezar();
				estado_actual = EST_MANTIENE_PULSADO;
			}
		//	} else {
		//		push_debug(EST_MANTIENE_PULSADO, 0);
		//	}
			break;
		}
		case EST_REBOTES_DEPRESION: { // Espera el tiempo TRD y habilita interrupciones
		//	tiempo_actual = tiempo_transcurrido(tiempo_referencia);
		//	if (tiempo_actual > TRD) {
			button_resetear(); // Habilita de nuevo las interrupciones para botón
			estado_actual = EST_INICIO;
			boton_antirrebotes_atendido = atendiendo_boton;
		//	} else {
		//		push_debug(EST_REBOTES_DEPRESION, 0);
		//	}
			break;
		}
		default: break;
    }
}
