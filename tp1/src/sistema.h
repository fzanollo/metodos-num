#include <vector>

using namespace std;

void printMatrix(vector<vector<double>>& c){
    int n = c.size();
    int m = c[0].size();

    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j)
        {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void printVector(vector<double>& v){
    for(auto e : v) {
        cout << e << ", ";
    }
    cout << endl;
}

vector<vector<double>> triangular(vector<vector<double>>& c){
    int m = c.size();
    int n = c[0].size();

    for (int i = 0; i < m-1 ; ++i) {
        for (int j = i+1; j < m; ++j) {

            double cij = c[i][j]/c[i][i];
            for (int k = i; k < n; ++k) {
                c[j][k] = c[j][k] - c[i][k] * cij;
            }
        }
    }

    return c;
}

vector<double> resolverSistLineal(vector<vector<double>>& c, vector<double>& b) {
    int n = c.size();
    vector<double> res(n);

    // aumento c
    for(int i = 0; i < n; i++) {
        c[i].push_back(b[i]);
    }

    triangular(c);

    for (int i = n-1; i >= 0; --i) {
        for (int j = n-1; j > i; --j) {
            res[i] -= c[i][j] * res[j];
        }
        res[i] = (res[i] + c[i][n]) / c[i][i];
    }

    return res;
}