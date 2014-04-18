Documentación de ev_fitness.cpp
===============================

Lo podeis encontrar en [ev_fitness.cpp] (https://github.com/rotty11/MiRepositorio/blob/master/ev_fitness.cpp). Lo que se ha hecho es desechar la estructura de booleanos del primer ejercicio y sustituirlo por una variante en la estructura de unsigned char a la hora de acceder a los cromosomas para el reconteo de unos:

  - Usando un unsigned char por cada cromosoma
  - Usando un unsigned char por cada 8 cromosomas (Obteniendo los unos sumando bit a bit)
  - Usando un unsigned char por cada 8 cromosomas (Obteniendo los unos consultando la lookup table)

#####Obtención de unos mediante unsigned char por cada cromosoma:

```cpp
// Voy comprobando si esta a 1 y lo sumo al contador
	// Eficiencia: O(n)
	// Ejecuciones: 2*n
	int cont = 0;
	for(itPOB1 = POB1.begin(); itPOB1 != POB1.end(); ++itPOB1)
		for(itGEN1 = (*itPOB1).begin(); itGEN1 != (*itPOB1).end(); ++itGEN1)
			if(*itGEN1 == 1)
				++cont;
```

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
