#include "botones_antirebotes.h"
#include "8led.h"
#include "button.h"
#include "stdint.h"


/*--------------------------------------------------------------*/

enum estado_jugada
{
    EST_COMIENZO_F,
    EST_SELECT_F,
    EST_COMIENZO_C,
    EST_SELECT_C,
};

static volatile enum estado_jugada estado_actual;
static volatile enum estado_button boton_actual;

void jugada_init() {
	estado_actual = EST_COMIENZO_F;
	D8Led_symbol(15); // F
}

uint8_t incrementar(uint8_t dato){
	//dato = (uint8_t)(dato+1u);
	dato++;
	if(dato == 9) {
		dato = 1;
	}

	return dato;
}

void jugada_por_botones(uint8_t* fila, uint8_t* columna, uint8_t* ready) {
	boton_actual = botones_antirebotes_leer();
	
    switch (estado_actual) {
		
    	case EST_COMIENZO_F:
		{
			if (boton_actual == button_iz) {
				D8Led_symbol(*fila); // 1
				estado_actual = EST_SELECT_F;
			}
			break;
		}
		
		case EST_SELECT_F: { 
			if(boton_antirrebotes_atendido == button_iz){ //incrementar
				*fila = incrementar(*fila);
				D8Led_symbol(*fila);
			}
		
			else if (boton_antirrebotes_atendido == button_dr) { // confirmar
				D8Led_symbol(12); // C
				estado_actual = EST_COMIENZO_C;
			}
			break;
		}
		
		case EST_COMIENZO_C: {
			if (boton_antirrebotes_atendido == button_iz) {
				D8Led_symbol(*columna); // 1
				estado_actual = EST_SELECT_C;
			}
			break;
		}
		
		case EST_SELECT_C:
		{
			if(boton_antirrebotes_atendido == button_iz){ //incrementar
				*columna = incrementar(*columna);
				D8Led_symbol(*columna);
			}
		
			else if (boton_antirrebotes_atendido == button_dr) { // confirmar
				*ready = 1;
				jugada_init();
			}
			break;
		}
    }
}
