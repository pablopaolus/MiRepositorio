#include <vector>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {

	// Compruebo el número de argumentos
	if(argc < 3) {
		printf("Uso: %s <Tamaño de población> <Número de cromosomas>\n", argv[0]);
		exit(-1);
	}
	srand(time(NULL));
	
	int tamPob = atoi(argv[1]);
	int numCro = atoi(argv[2]);
	struct timeval timPOB;

	// ----------------Implementacion con booleanos------------------
	vector< vector<bool> > POB1(tamPob, vector<bool>(numCro));
	vector< vector<bool> >::iterator itPOB1;
	vector<bool>::iterator itGEN1;

	gettimeofday(&timPOB, NULL);
	double dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy rellenando aleatoriamente con false o true el vector booleano
	for(itPOB1 = POB1.begin(); itPOB1 != POB1.end(); ++itPOB1)
		for(itGEN1 = (*itPOB1).begin(); itGEN1 != (*itPOB1).end(); ++itGEN1)
			if(rand() % 2 == 1)
				*itGEN1 = true;
			else
				*itGEN1 = false;

	gettimeofday(&timPOB, NULL);
	double dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("\nBooleanos: %.6lf Segundos\n", dTime2 - dTime1);

	// --------------Implementacion con unsigned char----------------
	vector< vector<unsigned char> > POB2(tamPob, vector<unsigned char>(numCro));
	vector< vector<unsigned char> >::iterator itPOB2;
	vector<unsigned char>::iterator itGEN2;
	
	gettimeofday(&timPOB, NULL);
	dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy rellenando aleatoriamente con 0 o 1 el vector de unsigned char
	for(itPOB2 = POB2.begin(); itPOB2 != POB2.end(); ++itPOB2)
		for(itGEN2 = (*itPOB2).begin(); itGEN2 != (*itPOB2).end(); ++itGEN2)
			*itGEN2 = rand() % 2;

	gettimeofday(&timPOB, NULL);
	dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("Unsigned char: %.6lf Segundos\n", dTime2 - dTime1);

	// -----------Implementacion con bits y unsigned char------------
	int numCro8 = numCro / 8;
	int rest8 = numCro % 8;
	bool exacto = (rest8 == 0) ? true : false;
	if(!exacto)
		++numCro8;
		
	vector< vector<unsigned char> > POB3(tamPob, vector<unsigned char>(numCro8));
	vector< vector<unsigned char> >::iterator itPOB3;
	vector<unsigned char>::iterator itGEN3;

	gettimeofday(&timPOB, NULL);
	dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB3 = POB3.begin(); itPOB3 != POB3.end(); ++itPOB3)
		for(itGEN3 = (*itPOB3).begin(); itGEN3 != (*itPOB3).end(); ++itGEN3)
			*itGEN3 = rand() % 256;

	gettimeofday(&timPOB, NULL);
	dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	printf("Unsigned char (bits): %.6lf Segundos\n\n", dTime2 - dTime1);
}
