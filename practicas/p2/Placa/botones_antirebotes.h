#ifndef _BOTONES_ANTIREBOTES_H_
#define _BOTONES_ANTIREBOTES_H_


#include "button.h"

void botones_antirebotes_gestion();
void boton_pulsado(enum estado_button boton_presionado);
enum estado_button botones_antirebotes_leer(void);
enum estado_button boton_antirrebotes_atendido;

#endif
