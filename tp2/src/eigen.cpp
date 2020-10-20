#include <algorithm>
#include <chrono>
#include <iostream>
#include "eigen.h"

using namespace std;

bool converge (const Vector & a,const Vector& b,double tolerancia){
    for (int i = 0; i < a.rows() ; ++i) {
        if ( !(-tolerancia < a(i)- b(i) && a(i)- b(i)  < tolerancia ) ){
            return false;
        }
    }
    return true;
}

pair<double, Vector> power_iteration(const Matrix& X, unsigned num_iter, double eps)
{
    Vector b = Vector::Random(X.cols());
    Vector b_viejo = b;
    double eigenvalue;
    for (unsigned int i = 0; i < num_iter; ++i) {
        b_viejo = b;
        b =  X*b ;
        b = b / b.norm();
        if(converge(b,b_viejo,eps)){
            //cout<< "Convergio en " << i << endl;
            break;
        }
    }
    eigenvalue = (b.transpose()*X)*b;
    eigenvalue = eigenvalue / (b.transpose()*b);

    return make_pair(eigenvalue, b );
}


pair<Vector, Matrix> get_first_eigenvalues(const Matrix& X, unsigned num, unsigned num_iter, double epsilon)
{
    Matrix A(X);
    Vector eigvalues(num);
    Matrix eigvectors(A.rows(), num);
    pair<double, Vector> pareja;
    for (unsigned int i = 0; i < num ; ++i) {
        pareja = power_iteration(A,num_iter,epsilon);
        eigvalues(i)= get<0>(pareja);
        eigvectors.col(i) = get<1>(pareja);
        A = A - eigvalues(i)*(eigvectors.col(i)*eigvectors.col(i).transpose());
    }
    return make_pair(eigvalues, eigvectors);
}

/*using  namespace std;
int main(int argc, char** argv){

    Matrix m = Matrix(3,3);
    Vector v = Vector(3);
    m << 3.0 , 0.0 , 0.0  ,
            0.0  , 2.0  , 0.0  ,
            0.0  , 0.0  , 1.0  ;
    v << 1.0,1.0,1.0;
    std::cout << m << std::endl;
    std::pair<double,Vector > ret = power_iteration(m,100000);
    std::cout << std::get<0>(ret) << std::endl;
    std::cout << std::get<1>(ret) << std::endl;
    cout  << "-----------------"<<endl;
    pair<Vector ,Matrix > todos = get_first_eigenvalues(m,3);
    std::cout << std::get<0>(todos) << std::endl;
    std::cout << std::get<1>(todos) << std::endl;
    return 0;
}*/


