#include "stdint.h"
#include "pila.h"
#include "button.h"
#include "botones_antirebotes.h"
#include "jugada.h"

// Tama�o del tablero
enum { DIM=8 };

// Valores que puede devolver la funci�n patron_volteo())
enum {
	NO_HAY_PATRON = 0,
  PATRON_ENCONTRADO = 1
};

enum {
	TABLEROS_NO_IGUALES = 0,
  TABLEROS_IGUALES = 1
};

enum {
	PATRONES_IGUALES = 0,
  PATRONES_NO_IGUALES = 1
};



// Estados de las casillas del tablero
// deberia ser enum, lo dejamos como const para usar uint8_t...
//const uint8_t CASILLA_VACIA = 0;
//const uint8_t FICHA_BLANCA  = 1;
//const uint8_t FICHA_NEGRA   = 2;

enum {
	CASILLA_VACIA = 0,
	FICHA_BLANCA = 1,
	FICHA_NEGRA = 2
};

// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
const uint8_t  NO              = 0;
const uint8_t  SI              = 1;
const uint8_t  CASILLA_OCUPADA = 2;

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean m�s f�ciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const uint8_t __attribute__ ((aligned (8))) tabla_valor[DIM][DIM] =
{
    {8,2,7,3,3,7,2,8},
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {3,4,5,0,0,5,4,3},
    {3,4,5,0,0,5,4,3},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {8,2,7,3,3,7,2,8}
};


// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
const int8_t vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
const int8_t vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deber�an serlo
// tablero, fila, columna y ready son varibles que se deber�an definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea m�s f�cil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
static uint8_t __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };

static uint8_t __attribute__ ((aligned (8))) tablero2[DIM][DIM];
static uint8_t __attribute__ ((aligned (8))) tablero3[DIM][DIM];

  ////////////////////////////////////////////////////////////////////
     // VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
     // Pregunta: �hay que hacer algo con ellas para que esto funcione bien?
     // (por ejemplo a�adir alguna palabra clave para garantizar que la sincronizaci�n a trav�s de esa variable funcione)
static volatile uint8_t // Poner volatile!!!
		 fila=0,
	    columna=0,
	    ready = 0;




// extern int patron_volteo_arm(uint8_t tablero[][8], int *longitud,uint8_t f, uint8_t c, uint8_t SF, uint8_t SC, uint8_t color);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(uint8_t tablero[][DIM], uint8_t candidatas[][DIM])
{
    int i, j;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
	for(j = 3; j < 5; ++j) {
	    tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
	}
    }

    for (i = 2; i < 6; ++i) {
	for (j = 2; j < 6; ++j) {
	    if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
		candidatas[i][j] = CASILLA_OCUPADA;
	    } else {
		candidatas[i][j] = SI; //CASILLA_LIBRE;
	    }
	}
    }
#endif
    // arriba hay versi�n alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionar�.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(uint8_t *ready)
{

	//autoasignar(); // Para test
    while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

void autoasignar() {
	fila = 2;
	columna = 3;
	ready = 1;
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posici�n indicadas por la fila y columna actual.
// Adem�s informa si la posici�n es v�lida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es v�lida o est� vacia.
uint8_t ficha_valida(uint8_t tablero[][DIM], uint8_t f, uint8_t c, int *posicion_valida)
{
    uint8_t ficha;

//    printf("%c, %c, %i",f, c, *posicion_valida);
    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que alg�n �ndice puede ser negativo

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}


////////////////////////////////////////////////////////////////////////////////
// La funci�n patr�n volteo comprueba si hay que actualizar una determinada direccion,
// busca el patr�n de volteo (n fichas del rival seguidas de una ficha del jugador actual)
// en una direcci�n determinada
// SF y SC son las cantidades a sumar para movernos en la direcci�n que toque
// color indica el color de la pieza que se acaba de colocar
// la funci�n devuelve PATRON_ENCONTRADO (1) si encuentra patr�n y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un par�metro por referencia. Sirve para saber la longitud del patr�n que se est� analizando.
//          Se usa para saber cuantas fichas habr�a que voltear
extern int patron_volteo_arm_c(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color);
extern int patron_volteo_arm_arm(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color);
int patron_volteo(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color)
{
	int posicion_valida; // indica si la posici�n es valida y contiene una ficha de alg�n jugador
	uint8_t casilla;   // casilla es la casilla que se lee del tablerotes();

	FA = FA + SF;
	CA = CA + SC;
	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	while ((posicion_valida == 1) && (casilla != color))
	// mientras la casilla est� en el tablero, no est� vac��a,
	// y es del color rival seguimos buscando el patron de volteo
	{
		FA = FA + SF;
		CA = CA + SC;
		*longitud = *longitud + 1;
		casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	}
    // si la ultima posici�n era v�lida y la ficha es del jugador actual,
    // entonces hemos encontrado el patr�n
	if ((posicion_valida == 1) && (casilla == color) && (*longitud >0)) {

		return PATRON_ENCONTRADO;
	}// si hay que voltear una ficha o m�s hemos encontrado el patr�n
	else {
		return NO_HAY_PATRON; // si no hay que voltear no hay patr�n
	}
}


////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la direcci�n que toque
// SF y SC son las cantidades a sumar para movernos en la direcci�n que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(uint8_t tablero[][DIM], uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, int n, uint8_t color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}


////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea v�lido
// f y c son la fila y columna a analizar
// uint8_t vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// uint8_t vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(uint8_t tablero[][DIM], uint8_t f, uint8_t c, uint8_t color)
{
    uint8_t SF, SC; // cantidades a sumar para movernos en la direcci�n que toque
    int i, flip, patron;
    volatile int tiempo = 0;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        // patron = patron_volteo_arm_c(tablero, &flip, f, c, SF, SC, color);
        // patron = patron_volteo_arm_arm(tablero, &flip, f, c, SF, SC, color);

        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
        i++;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posici�n si se puede mover
// En caso afirmativo, consulta la puntuaci�n de la posici�n y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(uint8_t candidatas[][DIM], uint8_t tablero[][DIM], uint8_t *f, uint8_t *c)
{
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    uint8_t mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    uint8_t SF, SC; // cantidades a sumar para movernos en la direcci�n que toque

    // Recorremos todo el tablero comprobando d�nde podemos mover
    // Comparamos la puntuaci�n de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {   // indica en qu� casillas quiz� se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento v�lido
                    // (es decir si implica voltear en alguna direcci�n)
                    while ((found == 0) && (k < DIM))
                    {
                        SF = vSF[k];    // k representa la direcci�n que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qu� hay que voltear en cada direcci�n
                        longitud = 0;
                        // patron = patron_volteo_arm_c(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        // patron = patron_volteo_arm_arm(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO)
                        {
                            found = 1;
                            if (tabla_valor[i][j] > mejor)
                            {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente direcci�n
                    }
                }
            }
        }
    }
    *f = (uint8_t) mf;
    *c = (uint8_t) mc;
    // si no se ha encontrado una posici�n v�lida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el n�mero de fichas de cada color.
// Los guarda en la direcci�n b (blancas) y n (negras)
void contar(uint8_t tablero[][DIM], int *b, int *n)
{
    int i,j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {
            if (tablero[i][j] == FICHA_BLANCA)
            {
                (*b)++;
            }
            else if (tablero[i][j] == FICHA_NEGRA)
            {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(uint8_t candidatas[][DIM], uint8_t f, uint8_t c)
{
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor s�
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f-1][c] != CASILLA_OCUPADA)
            candidatas[f-1][c] = SI;

        if ((c > 0) && (candidatas[f-1][c-1] != CASILLA_OCUPADA))
            candidatas[f-1][c-1] = SI;

        if ((c < 7) && (candidatas[f-1][c+1] != CASILLA_OCUPADA))
            candidatas[f-1][c+1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f+1][c] != CASILLA_OCUPADA)
            candidatas[f+1][c] = SI;

        if ((c > 0) && (candidatas[f+1][c-1] != CASILLA_OCUPADA))
            candidatas[f+1][c-1] = SI;

        if ((c < 7) && (candidatas[f+1][c+1] != CASILLA_OCUPADA))
            candidatas[f+1][c+1] = SI;
    }
    if ((c > 0) && (candidatas[f][c-1] != CASILLA_OCUPADA))
        candidatas[f][c-1] = SI;

    if ((c < 7) && (candidatas[f][c+1] != CASILLA_OCUPADA))
        candidatas[f][c+1] = SI;
}


int jugada_correcta(uint8_t fila, uint8_t columna){
	
}

////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la se�al de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deber�an ser locales de reversi8,
// la raz�n es que al meterlas en la pila no las pone juntas, y as� jugar es m�s complicado.
// en esta versi�n el humano lleva negras y la m�quina blancas
// no se comprueba que el humano mueva correctamente.
// S�lo que la m�quina realice un movimiento correcto.
void reversi8_main()
{

	uint32_t* stack_anterior = asignar_stack();
	uint32_t* evento;
	while(1) { // Fuera de reversi_init!


		 ////////////////////////////////////////////////////////////////////
		 // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
		// s�lo se exploran las que est�n alrededor de las fichas colocadas
		 ////////////////////////////////////////////////////////////////////
		uint8_t __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
		{
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO},
			{NO,NO,NO,NO,NO,NO,NO,NO}
		};


		int done;     // la m�quina ha conseguido mover o no
		int move = 0; // el humano ha conseguido mover o no
		int blancas, negras; // n�mero de fichas de cada color
		int fin = 0;  // fin vale 1 si el humano no ha podido mover
					  // (ha introducido un valor de movimiento con alg�n 8)
					  // y luego la m�quina tampoco puede
		uint8_t f, c;    // fila y columna elegidas por la m�quina para su movimiento
		volatile int tiempo = 0; // Para medidas de tiempo. Con volatile para que no se lo coma
								 // el compilador en -O3

		init_table(tablero, candidatas);

		stack_anterior--;

	    timer1_empezar();
		while (fin == 0)
		{
			jugada_init();
			fila = 1;
			columna = 1;
			ready = 0;
			while (ready == 0) {
				while(hay_eventos_encolados(stack_anterior) == 1) {

					evento = leer_evento(stack_anterior) ; // Desplazamos 24 bits a la derecha para quedarnos con ID_evento
					evento = (*evento) >> 24;

					if (evento == p_boton_der)  {
						button_ev_pulsacion(button_dr);
						jugada_por_botones(&fila, &columna, &ready);
					}
					else if (evento == p_boton_izq) {
						button_ev_pulsacion(button_iz);
						jugada_por_botones(&fila, &columna, &ready);
					}
					else if (evento == ev_timer0)	button_ev_tick0();
					else if (evento == ev_timer1)	Latido_ev_new_tick();
					else if (evento == ev_timer4)	button_ev_tick0();

				//	if (evento == EST_REBOTES_PRESION) botones_antirebotes_gestion();
				//	if (evento == EST_MANTIENE_PULSADO) botones_antirebotes_gestion();
				//	if (evento == EST_REBOTES_DEPRESION) botones_antirebotes_gestion();

					//if (boton_antirrebotes_atendido != button_none)

					stack_anterior -= 2;
					if (stack_anterior == 0x0c7ff404) { // 0x0c7ff504 = Stack End + 4
						stack_anterior = 0x0c7ff5fc; // = Stack Init - 4
					}

				}
			}

			move = 0;
			esperar_mov(&ready);
			// si la fila o columna son 8 asumimos que el jugador no puede mover
			if (((fila) != DIM) && ((columna) != DIM))
			{
				tablero[fila][columna] = FICHA_NEGRA;
				actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
				actualizar_candidatas(candidatas, fila, columna);
				move = 1;
			}

			// escribe el movimiento en las variables globales fila columna
			done = elegir_mov(candidatas, tablero, &f, &c);
			if (done == -1)
			{
				if (move == 0)
					fin = 1;
			}
			else
			{
				tablero[f][c] = FICHA_BLANCA;
				actualizar_tablero(tablero, f, c, FICHA_BLANCA);
				actualizar_candidatas(candidatas, f, c);
			}
		}
		contar(tablero, &blancas, &negras);

		dormir_procesador();
	}
}

void dormir_procesador() {

}


