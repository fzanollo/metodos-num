#pragma once
#include "types.h"

class PCA {
public:
    PCA(unsigned int n_components);

    void fit(Matrix X);
    void setTC(Matrix tc);
    Matrix getTC();

    Eigen::MatrixXd transform(Matrix X);

private:
	unsigned int n_components;
	Matrix tc;
};
