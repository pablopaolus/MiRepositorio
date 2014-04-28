#include <vector>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

// Funcion usada por qsort para ordenar por fitness a los individuos en el torneo
int compare(const void *ind1, const void *ind2) {

	return ( ((pair<int,int>*)ind2)->second - ((pair<int,int>*)ind1)->second );
}

int main(int argc, char *argv[]) {

	// Compruebo el numero de argumentos
	if(argc < 3) {
		printf("Uso: %s <Tamaño de población> <Número de cromosomas>\n", argv[0]);
		exit(-1);
	}
	srand(time(NULL));

	// Creacion de la tabla precomputada
	unsigned char BitsSetTable[256];
	BitsSetTable[0] = 0;
	for(int i = 0; i < 256; ++i)
		BitsSetTable[i] = (i & 1) + BitsSetTable[i / 2];

	// Leo los parametros	
	int tamPob = atoi(argv[1]);
	int numCro = atoi(argv[2]);
	struct timeval timPOB;

	// ----------Implementacion con bits y unsigned char------------
	int numCro8 = numCro / 8;
	int rest8 = numCro % 8;
	bool exacto = (rest8 == 0) ? true : false;
	if(!exacto)
		++numCro8;
		
	vector< vector<unsigned char> > POB(tamPob, vector<unsigned char>(numCro8));
	vector< vector<unsigned char> >::iterator itPOB;
	vector<unsigned char>::iterator itGEN;
	vector< pair<int,int> > TORNEO(tamPob); // Almaceno indice de individuo y su fitness (unos)

	gettimeofday(&timPOB, NULL);
	double dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB)
		for(itGEN = (*itPOB).begin(); itGEN != (*itPOB).end(); ++itGEN)
			*itGEN = rand() % 256;

	// Evaluo el fitness de cada individuo y lo almaceno en TORNEO
	int cont;
	for(int i = 0; i < tamPob; ++i) {
		cont = 0;
		for(int j = 0; j < numCro8; ++j) {
			unsigned char num = POB[i][j];
			while (num) {
				num &= (num-1);
				++cont;
			}
		}
		TORNEO[i] = make_pair(i,cont);
	}

	// Ordeno los individuos por fitness
	qsort(&TORNEO[0], tamPob, sizeof(pair<int,int>), compare);

	for(int i = 0; i < tamPob; ++i)
	printf("%d %d\n", TORNEO[i].first, TORNEO[i].second);

	// Realizo mutacion en cada individuo de la poblacion de 1 o varios bits
	/*for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB) {
		int cromosoma = rand() % numCro8;
		(*itPOB)[cromosoma] = rand() % 256;
	}

	// Realizo crossover entre dos individuos cogidos al azar (No me fijo en fitness ni nada)
	// Utilizo el crossover entre dos puntos tambien elegidos al azar
	// El resultado (dos hijos) lo almaceno en un nuevo vector de tamaño 2
	int ind1 = rand() % tamPob;
	int ind2 = rand() % tamPob;
	int p1 = rand() % numCro8;
	int p2 = rand() % numCro8;
	int menor = (p1<p2) ? p1 : p2;
	int mayor = (p1>p2) ? p1 : p2;
	vector< vector<unsigned char> > HIJOS;
	HIJOS.push_back(POB[ind1]);
	HIJOS.push_back(POB[ind2]);
	for(int i = menor; i <= mayor; ++i) {
		HIJOS[0][i] = POB[ind2][i];
		HIJOS[1][i] = POB[ind1][i];
	}*/
}
