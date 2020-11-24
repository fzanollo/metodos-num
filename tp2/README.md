# Reconocimiento de dígitos: TP2 de Métodos Numéricos
## 2020, 2do cuatrimestre

## Detalles de código
Para knn cambiamos la forma de utilizarlo, como explicamos en el informe, en función de correr los experimentos más rápido. En el main nos quedó "hardcodeado" el k máximo, en los experimentos desde las notebooks lo pasamos como parámetro.

## Experimentación
Se encuentra dentro de la carpeta notebooks, se corre de forma tradicional, está dividida entre las distintas .ipynb.

## CosoRecognizer
Es un programita hecho por nosotros, se puede dibujar y pedir una predicción. Para usarlo es necesario tkinter (apt-get install python3-tk), luego se debe ejecutar el script CosoRecognizer.py de la carpeta notebooks.

## Datos
En `data/` tenemos los datos de entrenamiento (`data/train.csv`) y los de test (`data/test.csv`).

## Instalación de las depencias
```
pip install -r requirements.txt
```

## Compilación
Ejecutar la primera celda del notebook `knn.ipynb` o seguir los siguientes pasos:

- Compilar el código C++ en un módulo de python
```
mkdir build
cd build
rm -rf *
cmake -DPYTHON_EXECUTABLE="$(which python)" -DCMAKE_BUILD_TYPE=Release ..
```
- Al ejecutar el siguiente comando se compila e instala la librería en el directorio `notebooks`
```
make install
```
