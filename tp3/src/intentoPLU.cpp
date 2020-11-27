/* Devuelve el índice de la fila tal que
   max{A[k][i]}, con i<=k<n
*/
int fila_max(const Matrix& A, int i) {
    int max = i; int n = A.rows();

    for (int k = i; k < n; k++) {
        if ( A(k)(i) > A(max)(i) ) {
            max = k;
        }
    }

    return max;
}

Matrix fact_PLU(Matrix A){

    int n = A.cols();
    Vector permut(A.cols() );
    for (int p= 0; p < A.cols() ; ++p) {
        permut(p) = p;
    }
    int pivot,i_viejo;
    for (int i = 0; i < n ; ++i) {
        /* pivoteo parcial */
        pivot = fila_max(A, i);
        if (pivot != i) {
            Vector viejo_Ai = A(i)    //A[i].swap(A[pivot]);
            A(i) = A(pivot)           //
            A(pivot) = viejo_Ai       // como cuesta swapear :?

            i_viejo = permut(i);
            permut(i) = pivot;
            permut(pivot) = i_viejo;
        }

        for (int j = i+1; j < n; ++j) {
            double m = A(j)(i)/A(i)(i);
            A(j)(i) = m; //L[j][i]

            for (int k = i+1; k < n ; ++k) {
                A(j)(k) = A(j)(k) - A(i)(k)*m;
            }
        }
    }

    Matrix B = Matrix(A.rows() +1, A.cols());
    B.topRows(A.rows()) = A.topRows(A.rows());
    B.bottomRows(1) =  permut;

    return B;
}


/* Resuelve un sistema de ecuaciones lineales Ax=b, 
 * donde A es una matriz triangular superior. 
 * El sistema se resuele mediante backward sustitution */
Vector solve_superior(Matrix & A, Vector& b,int fact) {
	int n = A.cols();                //A[0].size();

	Vector x = Zero(n);              //x(n, 0);
    for (int i = n-1; i >= 0; i--) {
        double sum = 0;
        for (int j = n-1; j > i; j--) {
            sum += A(i)(j)*x(j);
        }

        x(i) = (b(i) - sum) / A(i)(i);
    }
	return x;
}


/* Resuelve un sistema de ecuaciones lineales Ax=b, 
 * donde A es una matriz triangular inferior 
 * El sistema se resuele mediante forward sustitution */
Vector solve_inferior(Matrix& A, Vector& b,int fact) {

    int n = A.cols() ;//A[0].size();

	Vector x = Zero(n);              //x(n, 0);
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
            sum += A(i)(j) * x(j);
        }
        x(i) = ( b(A(n)(i)) - sum);                  //A[n]: P vector de permutaciones
    }

    return x;
}


Vector solve(Matrix& A, Vector& b, bool superior,int fact) {
	Vector x = Zero(n); 
	
    if(fact > 2 ){
	    throw invalid_argument("Factorizacion desconocida");
    }
	if (superior) {
        x = solve_superior(A, b, fact);
    }
	else {
        x = solve_inferior(A, b,fact);
    }
	return x;
}
