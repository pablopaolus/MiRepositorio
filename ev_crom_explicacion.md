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

De esta forma, dando distintos valores al número de cromosomas tenemos los siguientes tiempos de ejecución:

 Tam. población | Núm. cromosomas |    Bool    | Unsigned char | Unsigned char (bits)
----------------|-----------------|------------|---------------|---------------------
 	     100      |		     100      | 0.001232 s |  0.000696 s   |      0.000098 s
 	     100      |        500      | 0.002000 s |  0.001143 s   |      0.000146 s
 	     100      |       1000      | 0.003959 s |  0.002283 s   |      0.000288 s
