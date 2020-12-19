#include "stdint.h";

// Tamaño del tablero
enum { DIM=8 };

// Valores que puede devolver la función patron_volteo())
enum {
	NO_HAY_PATRON = 0,
  PATRON_ENCONTRADO = 1
};

enum {
	TABLEROS_NO_IGUALES = 0,
  TABLEROS_IGUALES = 1
};

enum {
	PATRONES_NO_IGUALES = 0,
	PATRONES_IGUALES = 1
};

enum { MOVIMIENTOS_TEST = 3 };


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
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
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
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
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

  ////////////////////////////////////////////////////////////////////
     // VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
     // Pregunta: ¿hay que hacer algo con ellas para que esto funcione bien?
     // (por ejemplo añadir alguna palabra clave para garantizar que la sincronización a través de esa variable funcione)
static uint8_t fila=0,
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
    // arriba hay versión alternativa
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
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionará.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(uint8_t *ready)
{
	// autoasignar();
    while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

void autoasignar() { // Para test

	//fila = 2;
	//columna = 3;
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
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
uint8_t ficha_valida(uint8_t tablero[][DIM], uint8_t f, uint8_t c, int *posicion_valida)
{
    uint8_t ficha;

//    printf("%c, %c, %i",f, c, *posicion_valida);
    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que algún índice puede ser negativo

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
// La función patrón volteo comprueba si hay que actualizar una determinada direccion,
// busca el patrón de volteo (n fichas del rival seguidas de una ficha del jugador actual)
// en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando.
//          Se usa para saber cuantas fichas habría que voltear
extern int patron_volteo_arm_arm(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color);
int patron_volteo(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color)
{
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	uint8_t casilla;   // casilla es la casilla que se lee del tablerotes();

	FA = FA + SF;
	CA = CA + SC;
	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	while ((posicion_valida == 1) && (casilla != color))
	// mientras la casilla está en el tablero, no está vací­a,
	// y es del color rival seguimos buscando el patron de volteo
	{
		FA = FA + SF;
		CA = CA + SC;
		*longitud = *longitud + 1;
		casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	}
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
	if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
	else
		return NO_HAY_PATRON; // si no hay que voltear no hay patrón
}



////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
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


int patron_volteo_test(uint8_t tablero[][DIM], int *longitud, uint8_t FA, uint8_t CA, uint8_t SF, uint8_t SC, uint8_t color) {

	int patron, patron2, patron3;

	patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
	longitud = 0;
    patron2 = patron_volteo_arm_c(tablero, &longitud, FA, CA, SF, SC, color);
	longitud = 0;
	patron3 = patron_volteo_arm_arm(tablero, &longitud, FA, CA, SF, SC, color);

	while(comprobar_patrones(patron, patron2, patron3) == 0) {}

	return patron;
}



////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// uint8_t vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// uint8_t vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(uint8_t tablero[][DIM], uint8_t f, uint8_t c, uint8_t color)
{
    uint8_t SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        // patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
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
    uint8_t SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos todo el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++)
    {
        for (j=0; j<DIM; j++)
        {   // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI)
            {
                if (tablero[i][j] == CASILLA_VACIA)
                {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM))
                    {
                        SF = vSF[k];    // k representa la dirección que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        // patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
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
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (uint8_t) mf;
    *c = (uint8_t) mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
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
    // En las posiciones alrededor sí
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

void reversi8_test()
{

	 ////////////////////////////////////////////////////////////////////
	 // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
	// sólo se exploran las que están alrededor de las fichas colocadas
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

    uint8_t __attribute__ ((aligned (8))) tablero_negras[MOVIMIENTOS_TEST][DIM][DIM] = {
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
    };

    uint8_t __attribute__ ((aligned (8))) tablero_blancas[MOVIMIENTOS_TEST][DIM][DIM] = {
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
        {
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_NEGRA,FICHA_NEGRA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
        },
    };

    uint8_t filas_negras[MOVIMIENTOS_TEST] = { 3, 1, 2 };
    uint8_t columnas_negras[MOVIMIENTOS_TEST] = { 2, 2, 3 };

    uint8_t filas_blancas[MOVIMIENTOS_TEST] = { 2, 2, 5 };
    uint8_t columnas_blancas[MOVIMIENTOS_TEST] = { 2, 4, 2 };
    int i = 0;

    init_table(tablero, candidatas);
    for (i = 0; i < MOVIMIENTOS_TEST; i++)
    {
        tablero[filas_negras[i]][columnas_negras[i]] = FICHA_NEGRA;
        actualizar_tablero(tablero, filas_negras[i], columnas_negras[i], FICHA_NEGRA);
        actualizar_candidatas(candidatas, filas_negras[i], columnas_negras[i]);

        while (comprobar_tableros(tablero_negras[i], tablero) == 0) {}

        tablero[filas_blancas[i]][columnas_blancas[i]] = FICHA_BLANCA;
        actualizar_tablero(tablero, filas_blancas[i], columnas_blancas[i], FICHA_BLANCA);
        actualizar_candidatas(candidatas, filas_blancas[i], columnas_blancas[i]);

        while (comprobar_tableros(tablero_blancas[i], tablero) == 0){}
     }
}


////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la señal de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deberían ser locales de reversi8,
// la razón es que al meterlas en la pila no las pone juntas, y así jugar es más complicado.
// en esta versión el humano lleva negras y la máquina blancas
// no se comprueba que el humano mueva correctamente.
// Sólo que la máquina realice un movimiento correcto.


void reversi8()
{

	// reversi8_test(); Función de testeo
	 ////////////////////////////////////////////////////////////////////
	 // Tablero candidatas: se usa para no explorar todas las posiciones del tablero
	// sólo se exploran las que están alrededor de las fichas colocadas
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


    int done;     // la máquina ha conseguido mover o no
    int move = 0; // el humano ha conseguido mover o no
    int blancas, negras; // número de fichas de cada color
    int fin = 0;  // fin vale 1 si el humano no ha podido mover
                  // (ha introducido un valor de movimiento con algún 8)
                  // y luego la máquina tampoco puede
    uint8_t f, c;    // fila y columna elegidas por la máquina para su movimiento
    volatile int tiempo = 0;

    init_table(tablero, candidatas);

    while (fin == 0)
    {
        move = 0;
      //  tiempo = timer2_parar();
        esperar_mov(&ready);
      //  timer2_inicializar();
      //  timer2_empezar();
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
}



int comprobar_tableros(uint8_t tablero1[][DIM], uint8_t tablero2[][DIM]) {
	int i = 0;
	int j = 0;

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			if(tablero1[i][j] != tablero2[i][j]){
				return TABLEROS_NO_IGUALES;
			}
		}
	}
	return TABLEROS_IGUALES;
}

int comprobar_patrones(int patron1, int patron2, int patron3) {
	if (patron1 == patron2 && patron1 == patron3 && patron2 == patron3) {
		return PATRONES_IGUALES;
	}
	return PATRONES_NO_IGUALES;
}

