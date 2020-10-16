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
	uint m = X.cols();

	Vector mu = (X.colwise().sum()) / n;
	cout << endl<< "mu " << mu << endl;

	// intento de hacerlo a mano, da mal
	// Matrix Xmu = Matrix(n,m);
	// for(auto row : X.transpose().colwise()){
	// 	cout << "otra: ";
	// 	cout << row << endl;
	// 	cout << (row - mu)/ sqrt(n-1) << endl;
 //    	Xmu << (row - mu)/ sqrt(n-1);
	// }

	// esta es la linea donde creo que esta el problema, use transpose porque eigen no te deja si pedis rowwise 
	Matrix Xmu = (X.transpose().colwise() - mu) / sqrt(n-1);
	cout << "Xmu " << Xmu << endl;
	
	Matrix Mcov = Xmu.transpose() * Xmu; // X.T * X 'o' X * X.T ???
	cout << "Mcov " << Mcov << endl;

	tc = get<0>(get_first_eigenvalues(Mcov, n_components)).transpose();
	cout << "tc " << tc;
}


MatrixXd PCA::transform(Matrix X)
{
	return tc * X;
}