#include "knn.h"

KNNClassifier::KNNClassifier(uint n_neighbors_max) {
    this->n_neighbors_max = n_neighbors_max;
}

void KNNClassifier::fit(Matrix X, Matrix y) {
    this->X = X; this->y = y;
}

void KNNClassifier::load(Matrix X) {
    D = Matrix(X.rows(), n_neighbors_max);

    // matriz de distancias
    for (uint k = 0; k < X.rows(); ++k) {
        // cout << "Vecinos más cercanos para elemento " << k << endl;
        D.row(k) = getNearestElements(X.row(k));
    }
}

Vector KNNClassifier::predict(uint n_neighbors) {
    assert(n_neighbors <= n_neighbors_max);
    auto ret = Vector(D.rows());

    for (uint k = 0; k < D.rows(); ++k) {
        ret(k) = resolve(D.row(k), n_neighbors);
        // if(k % 100 == 0) cout << "Listo " << k << "/" << D.rows() << " del set de validación" << endl; // <-- comentar si molesta
    }
    return ret;
}

Vector KNNClassifier::getNearestElements(Vector x) {
    point_queue queue(point_cmp);

    // comparaciones
    for(uint k = 0; k < X.rows(); k++) {
        double distance = (x.transpose() - X.row(k)).norm();
        uint label = y(k, 0);
        queue.push(point(distance, label));
    }

    auto ret = Vector(n_neighbors_max);

    for(uint k = 0; k < n_neighbors_max; k++) {
        ret(k) = queue.top().second;
        queue.pop();
    }

    return ret;
}

uint KNNClassifier::resolve(Vector nearest, uint n_neighbors) {
    const uint n_symbols = 10;
    uint votes[n_symbols] = {0};

    for(uint i = 0; i < n_neighbors; i++) {
        votes[(uint) nearest(i)]++;
    }

    uint label = 0;
    for(uint i = 0; i < n_symbols; i++) {
        label = votes[i] >= votes[label] ? i : label;
    }
    return label;
}
