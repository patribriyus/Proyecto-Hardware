/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- declaracion de funciones visibles del módulo button.c/button.h ---*/
enum estado_button {button_none, button_iz, button_dr};
enum estado_button button_estado(void);
void button_resetear();
void Eint4567_init(void);

#endif /* _BUTTON_H_ */
