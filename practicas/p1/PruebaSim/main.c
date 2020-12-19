#include <stdlib.h>
#define MAX_PACIENTES 16

struct Descriptor {
	unsigned int prioridad;
	unsigned int pid;
};


struct Descriptor Pacientes[MAX_PACIENTES] = { {127,1},{127,2},{112,3},{100,4},{132,5},{136,6},{255,7},{10,8},{0,0}};

struct Descriptor* Seleccionadas;
int nselec;

int NumPacientes(struct Descriptor* P)
{
	int i;

	for( i = 0; i < MAX_PACIENTES; i++ )
		if( (P[i].prioridad == 0) && ( P[i].pid == 0 ) )
			return i;

	return MAX_PACIENTES - 1;
}

int PosMinPrioridad(struct Descriptor* P,int ini, int num)
{
	int minpos = ini;
	int min = P[ini].prioridad;
	int i;

	for( i = ini+1; i < num; i++)
		if( P[i].prioridad < min ){
		   minpos = i;	
		   min = P[i].prioridad;
		}

	return minpos;
}

//Se da en ensamblador y se pide implementar en C
void Intercambiar(struct Descriptor* P, int i, int j);
/*
{
	struct Descriptor aux;

	aux.prioridad = P[i].prioridad;
	aux.pid = P[i].pid;

	P[i].prioridad = P[j].prioridad;
	P[i].pid = P[j].pid;

	P[j].prioridad = aux.prioridad;
	P[j].pid = aux.pid;
}
*/

//Se da en C y se pide implementar en ensamblador
void OrdenaPorPrioridad(struct Descriptor*  A, int num);
/*
{
	int i,j;

	for( i = 0; i < num ; i++ )
	{
		j = PosMinPrioridad(A,i,num);	
		Intercambiar(A,i,j);
	}
}
*/

// Describir y que se implemente en ensamblador
struct Descriptor* SeleccionarPID(struct Descriptor* P, int num, int* nselec, int prio );
/*
{
	int i=0;

	while( (P[i].prioridad < prio) && (i < num) ) i++;

	*nselec = num-i;
	
	if( i == num ) return NULL;
	else return &(P[i]);
}
*/

int main(void)
{
	int num;

	num = NumPacientes(Pacientes);

	OrdenaPorPrioridad( Pacientes, num );

	Seleccionadas = SeleccionarPID( Pacientes, num, &nselec, 127 );
			
	return 0;
}

