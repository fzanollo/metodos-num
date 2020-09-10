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

vector<vector<int>> calcularPartidos(int n, int m, vector<vector<int>>& queries, vector<double>& b){
    vector<vector<int>> cantPartidosPorEquipo(n, vector<int>(n));
    cout << "aca llego 2";
    
    for (int i = 0; i < m; ++i){
        int eq1 = queries[i][1];
        int eq2 = queries[i][3];
        cout << "aca llego: "<< i;

        
        cantPartidosPorEquipo[eq1][eq2]++;
        cantPartidosPorEquipo[eq2][eq1]++;

        b[eq1] += 1;
        b[eq2] -= 1;
    }

    return cantPartidosPorEquipo;
}

vector<long> compute(int n, int m, vector<vector<int>>& queries) {
    cout << "aca llego";
    vector<vector<double>> C;
    vector<double> b(m, 0.0);

    vector<vector<int>> cantPartidosPorEquipo = calcularPartidos(n, m, queries, b);

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j)
        {
            cout << cantPartidosPorEquipo[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;


    // TODO: armar C, b; resolver C * r = b; devolver r.
    vector<long> r(m);
    for(int i = 0; i < m; i++) {
        r[i] = queries[i][2];
    }
    return r;
}

void write(vector<long>& v, ofstream& fout) {
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
        queries[i].resize(4);

        for (int j = 0; j < 4; j++) {
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
