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
    KNNClassifier(uint n_neighbors_max);
    void fit(Matrix X, Matrix y);
    void load(Matrix X);
    Vector predict(uint n_neighbors);
    Matrix getD();
private:
    // point = par (distancia, label)
    typedef pair<double, uint> point;

    // comparador de point por distancia
    function<bool(point, point)> point_cmp = [](point left, point right) { return left.first > right.first; };

    // min queue para point (rápido para sacar el más cercano)
    typedef priority_queue<point, vector<point>, decltype(point_cmp)> point_queue;

    uint n_neighbors_max;
    Matrix X; // trainset
    Matrix y; // labels
    Matrix D; // distancias

    uint resolve(Vector, uint);

    Vector getNearestElements(Vector x);
};
