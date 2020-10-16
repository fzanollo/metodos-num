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
	//es col y no row porque eigen toma vectores verticales
	//cambiar por rowwise de ser necesario
	uint n = X.rows();
	// uint m = X.cols();

    Vector mu = X.colwise().mean();
	// Vector mu = (X.colwise().sum()) / n;
	cout << endl<< "mu " << mu << endl;

	Matrix Xmu = (X.rowwise() - mu.transpose()) / sqrt(n-1);
	cout << "Xmu " << Xmu << endl;
	
	Matrix Mcov = Xmu.transpose() * Xmu; // X.T * X 'o' X * X.T ???
	cout << "Mcov " << Mcov << endl;

	tc = (get_first_eigenvalues(Mcov, n_components)).second;
	cout << "tc " << tc;
}


MatrixXd PCA::transform(Matrix X)
{
	return tc * X;
}