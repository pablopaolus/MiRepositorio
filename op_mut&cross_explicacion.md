Documentación de op_mut&cross.cpp
===============================

Lo podeis encontrar en [op_mut&cross.cpp] (https://github.com/rotty11/MiRepositorio/blob/master/op_mut&cross.cpp). En este programa sólo se muestra la inicialización de los cromosomas a 1 ó 0 aleatoriamente y posteriormente se hará una mutación a uno o varios cromosomas. Después se realiza un crossover entre dos individuos cualesquiera usando la técnica del crossover en dos puntos. La estructura de datos usada será la mejor que se ha obtenido hasta ahora. Esto es:

  - Usando un unsigned char por cada 8 cromosomas

#####Mutación:
```cpp
// Realizo mutacion en cada individuo de la poblacion de 1 o varios bits
for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB) {
	int cromosoma = rand() % numCro8;
	(*itPOB)[cromosoma] = rand() % 256;
}
```
Lo que hacemos es mutar un unsigned char completo, aleatoriamente, lo que daría lugar a mutar en el mayor de los casos 8 bits, y por tanto 8 cromosomas. Así, en la representación binaria del podría conservarse el estado de algunos bits o sólo cambiaría uno, según el número que le toque. Ejemplo:

	Unsigned char anterior(Dec / Bin): 26 / 00011010
	Unsigned char generado(Dec / Bin): 51 / 00110011
	
Como se ve, cambian 3 bits. Por tanto, se han mutado 3 cromosomas.

#####Crossover en dos puntos:
```cpp
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
```
Aquí, lo que hacemos es operar a nivel de bits realizando AND lógica entre el número en cuestión y su anterior para obtener el bit menos significativo, sumándolo al contador y desplazando a la derecha todos los bits restantes. Todo ello tiene eficiencia `O(n*log2(num))` siendo `num` el valor del unsigned char y con un número de ejecuciones de `(n/8) + (n/8)*(log2(num)*3)`, que a efectos prácticos de media se reduce a `n/2` aproximadamente.

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
