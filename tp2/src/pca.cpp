#include <iostream>
#include "pca.h"
#include "eigen.h"

using namespace std;

PCA::PCA(unsigned int n_components)
{
	this->n_components = n_components;
}

void PCA::fit(Matrix X)
{
	uint n = X.rows();
	// uint m = X.cols();

    Vector mu = X.colwise().mean();
	Matrix Xmu = (X.rowwise() - mu.transpose()) / sqrt(n-1);
	
	Matrix Mcov = Xmu.transpose() * Xmu;

	tc = (get_first_eigenvalues(Mcov, n_components)).second;
	// cout << "tc " << tc;
}

void PCA::setTC(Matrix tc) {
	// tc tiene que venir ajustada para la cantidad de componentes que se desea
	this->tc = tc;
}

Matrix PCA::getTC() {
	return this->tc;
}

MatrixXd PCA::transform(Matrix X)
{
	return X * tc;
}