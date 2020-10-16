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
	uint dim = X.rows();

	Vector mu = (X.colwise().sum()) / dim;

	Matrix Xmu = (X.colwise() - mu) / sqrt(dim-1);

	Matrix Mcov = Xmu.transpose() * Xmu; // X.T * X 'o' X * X.T ???
	// cout << "Mcov " << Mcov << endl;

	tc = get<0>(get_first_eigenvalues(Mcov, n_components));
	// cout << "tc " << tc;
}


MatrixXd PCA::transform(Matrix X)
{
	return tc * X;
}
