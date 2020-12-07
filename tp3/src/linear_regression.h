#pragma once

#include "types.h"

class LinearRegression {
public:
    LinearRegression();

    void fit(Matrix X, Matrix y);

    Matrix predict(Matrix X);

    Vector getCoef();
private:
    Vector coef;
};
