Documentación de ev_crom.cpp
============================

Lo podeis encontrar en [ev_crom.cpp] (https://github.com/rotty11/MiRepositorio/blob/master/ev_crom.cpp). El programa está escrito en c++ y usando las librerías estándar de la STD. Lo que se ha hecho es comparar 3 estructuras distintas de datos. En todas ellas uso la clase `<vector>` para almacenar a la población y sus cromosomas:

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
  ./ev_crom <Tamaño de la población> <Número de cromosomas>
  ```

De esta forma, dando distintos valores al número de cromosomas tenemos los siguientes tiempos de ejecución (se han cogido las mejores mediciones. En mi fichero [ev_crom_tiempos.txt] (https://github.com/rotty11/MiRepositorio/blob/master/ev_crom_tiempos.txt) teneis todas las medidas de ejecución):

 Tam. población | Núm. cromosomas |    Bool    | Unsigned char | Unsigned char (bits)
----------------|-----------------|------------|---------------|---------------------
 	     100      |		     100      | 0.001232 s |  0.000696 s   |      0.000098 s
 	     100      |        500      | 0.002000 s |  0.001143 s   |      0.000146 s
 	     100      |       1000      | 0.003959 s |  0.002283 s   |      0.000288 s

Para 100 cromosomas, dado que son pocos, el tiempo comparado con 500 cromosomas no es proporcional. Pero entre 500 y 1000 ya sí lo es. De todas formas lo que nos interesa es la eficiencia entre estructuras y no entre tamaños del problema. Observamos cómo el peor es el Bool dado que es igual que el Unsigned char pero en el bucle hay que preguntar si es 0 ó 1 para transformarlo a false o true. En el último caso, siempre es casi 8 veces mejor que el Unsigned char a secas. Esto es debido a que sólo generamos la octava parte de números ya que un número se compone de 8 bits y por tanto 8 cromosomas de un tirón. Habría que ver, en posteriores ejercicios, si se puede conservar esta mejoría o si por el contrario al aplicar otro tipo de operaciones sobre los números empeoramos el tiempo.
Por último quiero agradecer a Paloma por su ayuda indirecta dado que me he basado en el raw de su fichero de explicación para poder formatear este texto (tabla, código, encabezado, enlaces) dado que yo no sabía nada del tema.
