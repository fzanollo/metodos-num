// TODO: mover esta doc a otra parte, o borrarla si no se necesita
// en src/tools
// python atp2mn.py
// en src/
// python metnum.py build
// cat ../data/atp_matches_2015.dat | OUTPUT_PATH=ranking.out ./tp

#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

vector<string> split_string(string);

void printMatrix(int n, int m, vector<vector<double>>& C){
    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j)
        {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//     i_max := argmax (i = h ... m, abs(A[i, k]))
int indexOfMaxRowInColumn(int n, int m, vector<vector<double>>& C, int h, int k){
    int i_max = h;
    for (int i = h; i < m; ++i){
        if (abs(C[i][k]) > abs(C[i_max][k])) i_max = i;
    }

    return i_max;
}

vector<vector<double>> triangular(int n, int m, vector<vector<double>>& C){
    int h = 0; //pivot row
    int k = 0; //pivot column

    int step = 1;

    while(h < m && k < n) {
        cout << "step: "<< step << " *********" << endl;
        step++;
        printMatrix(n, m, C);
        //find k-th pivot
        int i_max = indexOfMaxRowInColumn(n, m, C, h, k);

        if(C[i_max][k] != 0) {
            // swap h and i_max rows
            cout << "swap rows: " << h << " <-> " << i_max << endl;
            vector<double> auxRow = C[h];
            C[h] = C[i_max];
            C[i_max] = auxRow;

            printMatrix(n, m, C);

            for (int i = h+1; i < m; ++i){
                double f = C[i][k] / C[h][k];
                C[i][k] = 0;

                for (int j = k+1; j < n; ++j){
                    C[i][j] = C[i][j] - C[h][j]*f;
                }
            }
            h++;
        }
        
        k++;
    }

    return C;
}

map<int, vector<int>> calcularPartidos(int n, int m, vector<vector<int>>& queries){
    map<int, vector<int>> partidosPorEquipo; // eq -> [ganados, perdidos]

    for (int i = 0; i < m; ++i){
        int eq1 = queries[i][1];
        int ptseq1 = queries[i][2];
        int eq2 = queries[i][3];
        int ptseq2 = queries[i][4];

        // si el eq no esta en el map
        if(partidosPorEquipo.count(eq1)==0){
            partidosPorEquipo[eq1] = vector<int>(3);
        }

        if(partidosPorEquipo.count(eq2)==0){
            partidosPorEquipo[eq2] = vector<int>(3);
        }

        if(ptseq1 > ptseq2) {
            partidosPorEquipo[eq1][0]++;
            partidosPorEquipo[eq2][1]++;
        } else {
            partidosPorEquipo[eq1][1]++;
            partidosPorEquipo[eq2][0]++;
        }

        partidosPorEquipo[eq1][2]++;
        partidosPorEquipo[eq2][2]++;
    }

    return partidosPorEquipo;
}

vector<double> compute(int n, int m, vector<vector<int>>& queries) {
    vector<vector<double>> C;
    vector<double> b(m, 0.0);

    auto partidosPorEquipo = calcularPartidos(n, m, queries);

    for(auto par : partidosPorEquipo){
        int equipo = par.first;
        int ganados = par.second[0];
        int perdidos = par.second[1];
        int jugados = par.second[2];

        cout << equipo << " ganados: " << ganados << "; perdidos: " << perdidos << "; jugados: " << jugados << endl;
    }

    cout << endl;

    // TODO: armar C, b; resolver C * r = b; devolver r.


    vector<double> r(m);
    for(int i = 0; i < m; i++) {
        r[i] = queries[i][2];
    }
    return r;
}

void write(vector<double>& v, ofstream& fout) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        fout << *i << '\n';
    }
}

int main() {
    // TODO: levantar argv del enunciado

    ofstream fout(getenv("OUTPUT_PATH"));

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);


    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(5);

        for (int j = 0; j < 5; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    auto result = compute(n, m, queries);

    write(result, fout);

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
