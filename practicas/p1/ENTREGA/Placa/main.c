/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "timer2.h"
#include "44blib.h"
#include "44b.h"
#include "reversi8.h"

/*--- variables globales ---*/


/*--- codigo de funciones ---*/
void Main(void)
{
	unsigned int prueba;
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	//timer_init();	    // Inicializacion del temporizador
	//timer_init();
	timer2_inicializar();
	timer2_empezar();
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init();       // inicializamos el 8led

/*
	Delay(10000);

	prueba = timer2_parar();

	timer2_inicializar();
	timer2_empezar();
	Delay(1000);

	prueba = timer2_parar();

	/* Valor inicial de los leds */

	reversi8();

	leds_off();
	led1_on();

	while (1)
	{
		/* Cambia los leds con cada interrupcion del temporizador */
		if (switch_leds == 1)
		{

			leds_switch();
			switch_leds = 0;

		}

	}
}
