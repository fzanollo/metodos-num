#include "knn.h"

KNNClassifier::KNNClassifier(uint n_neighbors) {
    this->n_neighbors = n_neighbors;
}

void KNNClassifier::fit(Matrix X, Matrix y) {
    this->X = X; this->y = y;
}

Vector KNNClassifier::predict(Matrix X) {
    auto ret = Vector(X.rows());

    for (uint k = 0; k < X.rows(); ++k) {
        ret(k) = predictOne((Vector) X.row(k), KNNClassifier::electionMode);
        if(k % 100 == 0) cout << "Listo " << k << "/" << X.rows() << " del set de validación" << endl; // <-- comentar si molesta
    }
    return ret;
}

uint KNNClassifier::predictOne(Vector x, KNNClassifier::election_strategy_fn election) {
    // n elementos ordenados por distancia
    point_queue queue(point_cmp);

    // comparaciones
    for(uint k = 0; k < X.rows(); k++) {
        double distance = (x.transpose() - X.row(k)).norm();
        uint label = y(k, 0);
        queue.push(point(distance, label));
    }
    return election(queue, n_neighbors);
}

uint KNNClassifier::electionMode(point_queue queue, uint n_neighbors) {
    const uint n_symbols = 10;
    uint votes[n_symbols] = {0};

    for(uint i = 0; i < n_neighbors; i++) {
        votes[queue.top().second]++;
        queue.pop();
    }

    uint label = 0;
    for(uint i = 0; i < n_symbols; i++) {
        label = votes[i] >= votes[label] ? i : label;
    }
    return label;
}
