#pragma once

#include <algorithm>
//#include <chrono>
#include <iostream>
#include <utility>
#include <queue>
#include <functional>
#include "types.h"

using namespace std;

class KNNClassifier {
public:
    KNNClassifier(uint n_neighbors);
    void fit(Matrix X, Matrix y);
    Vector predict(Matrix X);
private:
    // point = par (distancia, label)
    typedef pair<double, uint> point;

    // comparador de point por distancia
    function<bool(point, point)> point_cmp = [](point left, point right) { return left.first > right.first; };

    // min queue para point (rápido para sacar el más cercano)
    typedef priority_queue<point, vector<point>, decltype(point_cmp)> point_queue;

    typedef function<uint(point_queue, uint)> election_strategy_fn;

    uint n_neighbors;
    Matrix X;
    Matrix y;

    // Predicción sólo para un elemento. "predict" lo hace para todas las rows de la matriz entrante.
    uint predict(Vector, election_strategy_fn);

    // Algoritmo de selección de label en base a la moda.
    static uint electionMode(point_queue, uint);
};
