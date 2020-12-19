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
#include "timer4.h"
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
	botones_antirebotes_init();

	timer_init();
	//timer_empezar();
	//timer1_init();
	//timer1_empezar();
	timer2_inicializar();

	timer4_init();

	timer2_empezar();
	timer4_empezar();



	//unsigned int tiempo = timer2_parar();

	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led

	// int interrupciones = interrupciones_var();
	// Inicializacion del temporizador

	D8Led_init();       // inicializamos el 8led



	/*
	// Timers completos
	int prueba0, prueba1, prueba2, prueba3, prueba4;
	timer2_inicializar();

	timer2_empezar();
	Delay(100);
	prueba1 = timer2_parar();

	timer2_empezar();
	Delay(1000);
	prueba2 = timer2_parar();

	timer2_empezar();
	Delay(10000);
	prueba3 = timer2_parar();

	timer2_empezar();
	Delay(100000);
	prueba4 = timer2_parar();

	prueba = timer2_parar();
*/

	while(1) {}
	definir_rutinas();
	// asm volatile (".word 0xe7f000f0\n");

	// while(1) {}
	reversi8_main();

	//timer1_init();

	/* Valor inicial de los leds */



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
