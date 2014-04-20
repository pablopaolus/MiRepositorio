#include <vector>
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

	int numCro8 = numCro / 8;
	int rest8 = numCro % 8;
	bool exacto = (rest8 == 0) ? true : false;
	if(!exacto)
		++numCro8;
		
	vector< vector<unsigned char> > POB(tamPob, vector<unsigned char>(numCro8));
	vector< vector<unsigned char> >::iterator itPOB;
	vector<unsigned char>::iterator itGEN;

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB)
		for(itGEN = (*itPOB).begin(); itGEN != (*itPOB).end(); ++itGEN)
			*itGEN = rand() % 256;

	// Realizo mutacion en cada individuo de la poblacion de 1 o varios bits
	for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB) {
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
	}
}
