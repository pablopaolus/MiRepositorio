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

	// Creacion de la tabla precomputada
	unsigned char BitsSetTable[256];
	BitsSetTable[0] = 0;
	for(int i = 0; i < 256; ++i)
		BitsSetTable[i] = (i & 1) + BitsSetTable[i / 2];

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB)
		for(itGEN = (*itPOB).begin(); itGEN != (*itPOB).end(); ++itGEN)
			*itGEN = rand() % 256;

	// Evaluo el fitness de cada individuo y lo almaceno en TORNEO
	int cont;
	long int Tfitness = 0;
	for(int i = 0; i < tamPob; ++i) {
		cont = 0;
		for(int j = 0; j < numCro8; ++j) {
			cont += BitsSetTable[POB[i][j]];
		}
		TORNEO[i] = make_pair(i,cont);
		Tfitness += cont;
	}

	// Ordeno los individuos por fitness
	qsort(&TORNEO[0], tamPob, sizeof(pair<int,int>), compare);

	/*printf("Individuos ordenados por fitness:\n");
	for(int i = 0; i < tamPob; ++i)
		printf("Num %d, fitness: %d\n", TORNEO[i].first, TORNEO[i].second);
	printf("Total fitness: %ld / %d\n\n", Tfitness, numCro * tamPob);*/

	// Ruleta donde almaceno los índices de los participantes en proporción a su fitness
	vector< int > RULETA(tamPob);
	int generados = 0;
	for(int i = 0; i < tamPob && generados < tamPob; ++i) {
		int copias = ((double) TORNEO[i].second / (double) Tfitness) * (double) tamPob;
		if(copias + generados > tamPob)
			copias = tamPob - generados;
		//printf("Copias de %d: %d\n", TORNEO[i].first, copias);
		for(int j = generados; j < copias + generados; ++j)
			RULETA[j] = TORNEO[i].first;
		generados += copias;
	}

	if(generados < tamPob)
		for(int i = generados, aux = 0; i < tamPob; ++i, ++aux)
			RULETA[i] = TORNEO[aux].first;

	/*printf("Indices de individuos en la ruleta: ");
	for(int i = 0; i < tamPob; ++i)
		printf("%d ", RULETA[i]);
	printf("\n");*/

	// Realizo crossover entre dos individuos cogidos al azar de entre la RULETA
	// Utilizo el crossover entre dos puntos tambien elegidos al azar
	// Cada hijo lo almaceno en un nuevo vector
	int cruces = tamPob >> 1;
	vector< vector<unsigned char> > HIJOS(cruces<<1);
	//printf("\nRealizo %d cruces:\n", cruces);
	for(int i = 0; i < cruces; ++i) {
		int ind1 = rand() % tamPob;
		int ind2 = rand() % tamPob;
		int p1 = rand() % numCro8;
		int p2 = rand() % numCro8;
		int menor = (p1<p2) ? p1 : p2;
		int mayor = (p1>p2) ? p1 : p2;
		HIJOS[i<<1] = POB[RULETA[ind1]];
		HIJOS[(i<<1)+1] = POB[RULETA[ind2]];
		for(int j = menor; j <= mayor; ++j) {
			HIJOS[i<<1][j] = POB[RULETA[ind2]][j];
			HIJOS[(i<<1)+1][j] = POB[RULETA[ind1]][j];
		}
		/*printf("\nCruce %d: entre padres %d y %d con cruce entre los puntos %d y %d\n", i, RULETA[ind1], RULETA[ind2], menor, mayor);
		printf("PADRE %d: ", RULETA[ind1]);
		for(int k = 0; k < numCro8; ++k) {
			printf("%d ", POB[RULETA[ind1]][k]);
		}
		printf("\n");
		printf("PADRE %d: ", RULETA[ind2]);
		for(int k = 0; k < numCro8; ++k) {
			printf("%d ", POB[RULETA[ind2]][k]);
		}
		printf("\n");
		printf("HIJO %d: ", i<<1);
		for(int k = 0; k < numCro8; ++k) {
			printf("%d ", HIJOS[i<<1][k]);
		}
		printf("\n");
		printf("HIJO %d: ", (i<<1)+1);
		for(int k = 0; k < numCro8; ++k) {
			printf("%d ", HIJOS[(i<<1)+1][k]);
		}
		printf("\n\n");*/
	}

	// Realizo mutacion en cada hijo de 1 o varios bits
	for(itPOB = HIJOS.begin(); itPOB != HIJOS.end(); ++itPOB) {
		int cromosoma = rand() % numCro8;
		(*itPOB)[cromosoma] = rand() % 256;
	}

	// Si la poblacion es impar el numero de hijos generados es uno menos: Agregamos entonces al padre con mas fitness
	// Ya tenemos la nueva población generada
	if(tamPob % 2 == 1)
		HIJOS.push_back(POB[TORNEO[0].first]);

	// Imprimo en pantalla los hijos definitivos
	/*printf("Nueva poblacion:\n");
	for(itPOB = HIJOS.begin(); itPOB != HIJOS.end(); ++itPOB) {
		for(itGEN = (*itPOB).begin(); itGEN != (*itPOB).end(); ++itGEN)
			printf("%d ", *itGEN);
		printf("\n");
	}*/

	gettimeofday(&timPOB, NULL);
	double dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("Algoritmo genético: %.6lf Segundos\n\n", dTime2 - dTime1);
}
