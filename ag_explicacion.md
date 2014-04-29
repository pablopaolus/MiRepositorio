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

#####Obtención de unos mediante el método de la Lookup Table:
```cpp
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
```
Para usar este método, lo primero que tenemos que crear es una tabla inicializada con la acumulación de los unos que tiene cada unsigned char (0-255). De esta forma, cada vez que consultemos un número lo único que tenemos que hacer es buscar en la tabla cuántos unos tiene. De esta forma, por un lado ganamos tiempo al tener n/8 unsigned char respecto al primer método y también ganamos tiempo porque para cada unsigned char basta con un acceso en obtener su número de unos (en vez de ir bit a bit como en el segundo método). Todo ello tiene una eficiencia `O(n)` y con un número de ejecuciones de `2*(n/8) = n/4`.

Ejecución del programa
----------------------

  ```bash
  ./ev_fitness <Tamaño de la población> <Número de cromosomas>
  ```

De esta forma, dando distintos valores al número de cromosomas tenemos los siguientes tiempos de ejecución (se han cogido las mejores mediciones. En mi fichero [ev_fitness_tiempos.txt] (https://github.com/rotty11/MiRepositorio/blob/master/ev_fitness_tiempos.txt) teneis todas las medidas de ejecución):

 Tam. población | Núm. cromosomas | Unsigned char | Unsigned char (Brian Kernighan) | Unsigned char (Lookup Table)
----------------|-----------------|---------------|---------------------------------|-----------------------------
       100      |        128      |  0.000873 s   |            0.000181 s           |          0.000094 s
       100      |        256      |  0.001734 s   |            0.000354 s           |          0.000174 s
       100      |       1024      |  0.002298 s   |            0.000459 s           |          0.000219 s

Observamos en la tabla que aproximadamente se respetan los tiempos de ejecución con las fórmulas mencionadas anteriormente. El método de Brian Kernighan reduce unas 4,7 veces el tiempo anterior si tenemos en cuenta que sólo rellena la octava parte de cromosomas pero usa más tiempo al tener que operar a nivel de bits varias veces por cada unsigned char. En última instancia, en el método de la Lookup Table, hay que mencionar que para tamaños pequeños de población o número de cromosomas el tiempo empeoraría debido a que hay que crear previamente la tabla y no compensa, pero en estos casos es lo suficientemente grande como para poder sacar ventaja. Como conclusión, el último método es el mejor en estos casos y lo validamos como el más eficiente.
