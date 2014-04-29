Documentación de ag.cpp
=======================

Lo podeis encontrar en [ag.cpp] (https://github.com/rotty11/MiRepositorio/blob/master/ag.cpp). Aquí lo que se ha hecho es ejecutar un algoritmo genético completo (pasos básicos) para la creación de una nueva población de individuos en base a los ya existentes. Como estructura de datos, se ha usado la mejor hasta ahora:

  - Usando un unsigned char por cada 8 cromosomas

Esto sería:

```cpp
vector< vector<unsigned char> > POB(tamPob, vector<unsigned char>(numCro8));
```

Donde `tamPob` es el tamaño de la población y `numCro8` es el número de cromosomas por individuo entre 8 dado que un unsigned char me codifica 8 cromosomas.

#####Inicialización de la población:
```cpp
// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB)
	for(itGEN = (*itPOB).begin(); itGEN != (*itPOB).end(); ++itGEN)
		*itGEN = rand() % 256;
```

#####Evaluación de la población inicial en función a su fitness:
El fitness usado es el reconteo de unos en los cromosomas (COUNT ONES) utilizando la técnica de la Lookup Table. Acto seguido, en una variable llamada `TORNEO` almaceno los índices que poseían los individuos en el vector `POB` y su fitness. Por último ordeno el torneo `TORNEO` en base al fitness usando `qsort`.

```cpp
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
```

#####Selección:
Como tenemos ordenada la población en base al fitness, procedemos a crear la ruleta. Para ello, lo que hacemos es crear otro vector de tamaño igual a la población `tamPob` llamado `RULETA` y crear copias de los individuos proporcionalmente a su fitness. Pero para no tener que copiar tantos datos, el vector sólo contendrá los índices. Si el número de copias no llega a `tamPob`, se rellena lo que falte con una copia extra de cada individuo empezando por el que más fitness tenía. Para mostrar todo esto, se ha dejado un fichero llamado [ag_log.txt] (https://github.com/rotty11/MiRepositorio/blob/master/ag_log.txt) donde se puede ver la traza de todo el algoritmo para que veais cómo funciona. A continuación se muestra el código de todo lo explicado anteriormente.

```cpp
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
```

#####Crossover:
Lo que hacemos en el crossover es coger aleatoriamente dos números entre 0 y `tamPob` que serán los índices a coger del vector `RULETA`. Como en `RULETA` hay más índices o menos según el fitness de cada individuo, hay más probabilidad de que el índice seleccionado pertenezca a individuos con más fitness.
Una vez seleccionados los padres, obtenemos aleatoriamente dos puntos que definirán el trozo de cadena a intercambiar a la hora de generar los hijos. Lo bueno de cogerlos al azar radica en que si en un momento determinado se cruzasen los mismos padres más de una vez, al cruzar distintos trozos de cadena obtendríamos distintos hijos. Los hijos se almacenan en un nuevo vector llamado `HIJOS` conteniendo a la nueva población. El código siguiente refleja lo mostrado:
```cpp
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
```

#####Mutación:
Aplicamos una mutación de 1 o varios bits en un individuo de forma aleatoria:

```cpp
// Realizo mutacion en cada hijo de 1 o varios bits
for(itPOB = HIJOS.begin(); itPOB != HIJOS.end(); ++itPOB) {
	int cromosoma = rand() % numCro8;
	(*itPOB)[cromosoma] = rand() % 256;
}
```

#####Completar la población:
Si el tamaño de población fuese impar, el número de cruces sería la parte entera de la mitad. Como cada cruce crea dos hijos, nos faltaría un hijo para llegar a completar `tamPob` individuos. En este caso, el individuo que falta será el padre con más fitness de la generación "perdida" (que acaba de ser sustituída). El código siguiente lo manifiesta:

```cpp
// Si la poblacion es impar el numero de hijos generados es uno menos: Agregamos entonces al padre con mas fitness
// Ya tenemos la nueva población generada
if(tamPob % 2 == 1)
	HIJOS.push_back(POB[TORNEO[0].first]);
```

Ejecución del programa
----------------------

  ```bash
  ./ag <Tamaño de la población> <Número de cromosomas>
  ```

De esta forma, dando distintos valores al número de cromosomas tenemos los siguientes tiempos de ejecución (se han cogido las mejores mediciones. En mi fichero [ag_tiempos.txt] (https://github.com/rotty11/MiRepositorio/blob/master/ag_tiempos.txt) teneis todas las medidas de ejecución):

 Tam. población | Núm. cromosomas | Unsigned char | Unsigned char (Brian Kernighan) | Unsigned char (Lookup Table)
----------------|-----------------|---------------|---------------------------------|-----------------------------
       100      |        128      |  0.000873 s   |            0.000181 s           |          0.000094 s
       100      |        256      |  0.001734 s   |            0.000354 s           |          0.000174 s
       100      |       1024      |  0.002298 s   |            0.000459 s           |          0.000219 s

Observamos en la tabla que aproximadamente se respetan los tiempos de ejecución con las fórmulas mencionadas anteriormente. El método de Brian Kernighan reduce unas 4,7 veces el tiempo anterior si tenemos en cuenta que sólo rellena la octava parte de cromosomas pero usa más tiempo al tener que operar a nivel de bits varias veces por cada unsigned char. En última instancia, en el método de la Lookup Table, hay que mencionar que para tamaños pequeños de población o número de cromosomas el tiempo empeoraría debido a que hay que crear previamente la tabla y no compensa, pero en estos casos es lo suficientemente grande como para poder sacar ventaja. Como conclusión, el último método es el mejor en estos casos y lo validamos como el más eficiente.
