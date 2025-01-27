/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*--- variables visibles del m�dulo timer.c/timer.h ---*/
int switch_leds;

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/
void timer_init2(void);
void timer2_inicializar(void);
void timer2_empezar(void);
unsigned int timer2_leer();
unsigned int timer2_parar();

#endif /* _TIMER_H_ */
