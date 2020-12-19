#include "latido.h"
#include "led.h"
#include "timer1.h"
#include "stdio.h"

static volatile int interrupciones = 0;
static volatile enum estado_led {led_on, led_off};
static volatile enum estado_led estado_actual_led = led_off;

void Latido_ev_new_tick(void) {
    interrupciones++;

	if (estado_actual_led == led_on) {
		if(interrupciones == 7) { // 7 interrupciones para apagar el LED
			led1_off();
			estado_actual_led = led_off;
			interrupciones = 0;
			//timer1_empezar();
		}
/**		if (interrupciones == 8) { // 8 interrupciones para encender el LED
			if (estado_actual_led == led_off) {
				led1_on();
				estado_actual_led = led_on;
				interrupciones = 0;
				//timer1_empezar();
			}
		}*/
    }
	if (estado_actual_led == led_off) {
		if(interrupciones == 8) { // 7 interrupciones para apagar el LED
			led1_on();
			estado_actual_led = led_on;
			interrupciones = 0;
			//timer1_empezar();
		}
/*		if (interrupciones == 8) { // 8 interrupciones para encender el LED
			if (estado_actual_led == led_off) {
				led1_on();
				estado_actual_led = led_on;
				interrupciones = 0;
				//timer1_empezar();
			}
		}*/
	}
}



