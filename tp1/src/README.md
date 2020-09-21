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

## Error absoluto
Todos los scripts para crear/correr/comparar/generar gráficos se encuentran en la notebook llamada tp1mn

## Análisis cualitativo

Para correr cada caso planteado se deben utilizar los `.dat` que están en `data/casos/`. Por ejemplo:

```bash
$ ./tp ../data/casos/caso1.dat ranking.out 0
```
