# Catedra
Para correr los scripts, desde una consola ejecutar:

```bash
$ python metnum.py opcion
```

donde opcion puede ser:

__compile__: Compila todos los *.cpp que hayan en el directorio.

__link__: Genera el ejecutable en base a los *.o generados previamente.

__build__: compile + link

__clean__: borra los *.o y el ejecutable.

__test__: hace el build, busca lo archivos *.in en la carpeta tests/, ejecuta el programa y guarda el
resultado para cada corrida en el correspondiente .out. Despues, chequea que el resultado sea el "mismo" que el
.expected, tambien del directorio test. En este caso, la comparacion es por tolerancia coordenada a coordeanda del vector
solucion.


# ¿Cómo replicar las pruebas que se muestran en el informe?

Observación: para correr el tp el orden esperado de los parámetros es ./tp inputfile outputfile algorithm (donde algorithm puede ser 0-CMM, 1-WP, los resultados de Elo fueron generados utilizando python)

#### Crear los .dat para las notebooks y correr los test

```bash
.../tools$ python atp2mn.py
.../tools$ python massey2mn.py
.../src$ python metnum.py test
```

#### ¿Qué hay en las notebooks?

Parte del desarrollo teórico de las respuestas del informe puede ser encontrado en las mismas, puede que haya algún error. Dado que las mismas están pasadas en el informe en forma, pedimos desestimarlas de ser necesario. Por otro lado, los scripts para las pruebas que presentamos en el pdf final se encuentran ahí también, como para correr.

## Pruebas
__Error absoluto__: Todos los scripts para crear/correr/comparar/generar gráficos se encuentran en la notebook llamada tp1mn.

Las pruebas relacionadas a __ATP 2015__ se encuentran en la notebook expAtp.

Las pruebas relacionadas a __NBA 2016__ se encuentran en la notebook nba.

Para correr cada caso planteado en la __prueba por casos__ se deben utilizar los `.dat` que están en `data/casos/`. Por ejemplo:

```bash
.../src$ ./tp ../data/casos/caso1.dat ranking.out 0
```
