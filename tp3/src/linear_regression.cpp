#include <algorithm>
//#include <chrono>
#include <pybind11/pybind11.h>
#include <iostream>
#include <exception>
#include "linear_regression.h"

using namespace std;
namespace py=pybind11;

LinearRegression::LinearRegression()
{
    
}

void LinearRegression::fit(Matrix X, Matrix y)
{
    //Quiero resolver el sistemas (X^t)X(solucion) = (X^t)y 
    Matrix A = X.transpose() * X;
    Vector b = X.transpose() * y;
    Vector x = A.householderQr().solve(b);
    this->coef = x;

}


Matrix LinearRegression::predict(Matrix X)
{
    //auto ret = MatrixXd::Zero(X.rows(), 1);
    //return ret;
    return X * this->coef;
}

Vector LinearRegression::getCoef()
{
    return this->coef;
}