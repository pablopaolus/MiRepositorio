Documentación de ev_crom.cpp
============================

El programa está escrito en c++ y usando las librerías estándar de la STD. Lo que se ha hecho es comparar 3 estructuras distintas de datos. En todas ellas uso la clase `<vector>` para almacenar a la población y sus cromosomas:

  - Usando booleanos
  - Usando un unsigned char por cada cromosoma
  - Usando un unsigned char por cada 8 cromosomas

Pasándolo a código, respectivamente, sería:

  ```c++
  vector< vector<bool> > POB1(tamPob, vector<bool>(numCro));
  vector< vector<unsigned char> > POB2(tamPob, vector<unsigned char>(numCro));
  vector< vector<unsigned char> > POB3(tamPob, vector<unsigned char>(numCro8));
  ```
  Donde `tamPob` es el tamaño de la población y `numCro` es el número de cromosomas por individuo.

La explicación es sencilla:

  - En el primero, cada cromosoma toma false o true. Un booleano ocupa 1 byte de memoria
  - En el segundo, cada cromosoma toma 0 o 1. Un unsigned char ocupa 1 byte de memoria
  - En el tercero, cada unsigned char es un 1 byte = 8 bits, y cada bit será un cromosomas. De esta forma el vector se reduce a la octava parte.

Ejecución del programa
---------------------------

  ```bash
  ./ev_crom.cpp <Tamaño de la población> <Número de cromosomas>
  ```

De esta forma, dando distintos valores a los argumentos del programa tenemos los siguientes tiempos de ejecución:

 Tamaño de la población | Booleanos               | Unsigned char      | Unsigned char (bits)
------------------------|------------------------ |--------------------|---------------------
 	100                   |		  0.000294 segundos   | 0.000442 segundos  | 0.000442 segundos
 	1000                  |  		0.00216 segundos    | 0.004022 segundos  | 0.00023 segundos
 	1000                  |  		0.022526 segundos   | 0.037748 segundos  | 0.39933 segundos

