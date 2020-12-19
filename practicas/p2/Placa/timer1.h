/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER1_H_
#define _TIMER1_H_

/*--- variables visibles del módulo timer.c/timer.h ---*/
int switch_leds;

/*--- declaracion de funciones visibles del módulo timer.c/timer.h ---*/
void timer1_init(void);
void timer1_empezar(void);

#endif /* _TIMER_H_ */
