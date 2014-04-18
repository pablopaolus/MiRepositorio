#include <vector>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {

	// Compruebo el numero de argumentos
	if(argc < 3) {
		printf("Uso: %s <Tamaño de población> <Número de cromosomas>\n", argv[0]);
		exit(-1);
	}
	srand(time(NULL));
	
	int tamPob = atoi(argv[1]);
	int numCro = atoi(argv[2]);
	struct timeval timPOB;

	// --------------Implementacion con unsigned char---------------
	vector< vector<unsigned char> > POB1(tamPob, vector<unsigned char>(numCro));
	vector< vector<unsigned char> >::iterator itPOB1;
	vector<unsigned char>::iterator itGEN1;

	// Voy rellenando aleatoriamente con 0 o 1 el vector de unsigned char
	for(itPOB1 = POB1.begin(); itPOB1 != POB1.end(); ++itPOB1)
		for(itGEN1 = (*itPOB1).begin(); itGEN1 != (*itPOB1).end(); ++itGEN1)
			*itGEN1 = rand() % 2;

	gettimeofday(&timPOB, NULL);
	double dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy comprobando si esta a 1 y lo sumo al contador
	// Eficiencia: O(n)
	// Ejecuciones: 2*n
	int cont = 0;
	for(itPOB1 = POB1.begin(); itPOB1 != POB1.end(); ++itPOB1)
		for(itGEN1 = (*itPOB1).begin(); itGEN1 != (*itPOB1).end(); ++itGEN1)
			if(*itGEN1 == 1)
				++cont;

	gettimeofday(&timPOB, NULL);
	double dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("\nUnsigned char: %.6lf Segundos\n", dTime2 - dTime1);

	// ----------Implementacion con bits y unsigned char------------

	int numCro8 = numCro / 8;
	int rest8 = numCro % 8;
	bool exacto = (rest8 == 0) ? true : false;
	if(!exacto)
		++numCro8;
		
	vector< vector<unsigned char> > POB2(tamPob, vector<unsigned char>(numCro8));
	vector< vector<unsigned char> >::iterator itPOB2;
	vector<unsigned char>::iterator itGEN2;

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB2 = POB2.begin(); itPOB2 != POB2.end(); ++itPOB2)
		for(itGEN2 = (*itPOB2).begin(); itGEN2 != (*itPOB2).end(); ++itGEN2)
			*itGEN2 = rand() % 256;

	gettimeofday(&timPOB, NULL);
	dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy comprobando cada bit de cada unsigned char si está a 1 y lo sumo al contador (Brian Kernighan)
	// Eficiencia: O(n*log2(num)) -->>> OJO!! n/8 operaciones básicas + log2(num)
	// Ejecuciones: (n/8) + (n/8)*(log2(num)*3) -->>> de media n/2
	cont = 0;
	for(itPOB2 = POB2.begin(); itPOB2 != POB2.end(); ++itPOB2)
		for(itGEN2 = (*itPOB2).begin(); itGEN2 != (*itPOB2).end(); ++itGEN2) {
			unsigned char num = *itGEN2;
			while (num) {
				num &= (num-1);
				++cont;
			}
		}

	gettimeofday(&timPOB, NULL);
	dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("Unsigned char (bits) Brian Kernighan: %.6lf Segundos\n", dTime2 - dTime1);

	gettimeofday(&timPOB, NULL);
	dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Creacion de la tabla precomputada
	unsigned char BitsSetTable[256];
	BitsSetTable[0] = 0;
	for(int i = 0; i < 256; ++i)
		BitsSetTable[i] = (i & 1) + BitsSetTable[i / 2];

	// Voy comprobando en la tabla cuantos unos tiene cada unsigned char y lo sumo al contador (Lookup Table)
	// Eficiencia: O(n)
	// Ejecuciones: 2*(n/8) = n/4
	cont = 0;
	for(itPOB2 = POB2.begin(); itPOB2 != POB2.end(); ++itPOB2)		
		for(itGEN2 = (*itPOB2).begin(); itGEN2 != (*itPOB2).end(); ++itGEN2)
			cont += BitsSetTable[*itGEN2];

	gettimeofday(&timPOB, NULL);
	dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("Unsigned char (bits) Lookup Table: %.6lf Segundos\n\n", dTime2 - dTime1);
}
