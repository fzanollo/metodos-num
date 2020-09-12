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

void printVector(vector<double>& v){
    for(auto e : v) {
        cout << e << ", ";
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
        // cout << "step: "<< step << " *********" << endl;
        step++;
        // printMatrix(n, m, C);
        //find k-th pivot
        int i_max = indexOfMaxRowInColumn(n, m, C, h, k);

        if(C[i_max][k] != 0) {
            // swap h and i_max rows
            // cout << "swap rows: " << h << " <-> " << i_max << endl;
            vector<double> auxRow = C[h];
            C[h] = C[i_max];
            C[i_max] = auxRow;

            // printMatrix(n, m, C);

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

vector<double> resolver(vector<vector<double>>& c, vector<double>& b) {
    int n = c.size();

    vector<double> r(n);

    // aumento c

    for(int i = 0; i < n; i++) {
        c[i].push_back(b[i]);
    }

    triangular(n+1, n, c);

    n--;

    r[n] = c[n][n+1] / c[n][n];

    for(int i = n-1; i >= 0; i--) {
        double sum = 0;
        for(int j = i+1; j <= n; j++) {
            sum += c[i][j] * r[j];
        }
        r[i] = (c[i][n+1] - sum) / c[i][i];
    }

    return r;
}

map<int, map<int, vector<int>>> calcularPartidos(int n, int m, vector<vector<int>>& queries, set<int>& equiposIds){
    map<int, map<int, vector<int>>> partidosPorEquipo; // eq -> eq contrincante -> [ganados, perdidos]
    
    for (int i = 0; i < m; ++i){
        int eq1 = queries[i][1];
        int eq2 = queries[i][3];

        // si el eq no esta en el map
        if(partidosPorEquipo.count(eq1)==0){
            partidosPorEquipo[eq1] = map<int,vector<int>>();
            equiposIds.insert(eq1);
        } 
        if(partidosPorEquipo.count(eq2)==0){
            partidosPorEquipo[eq2] = map<int,vector<int>>();
            equiposIds.insert(eq2);
        } 

        // si el eq no esta en el submap
        if(partidosPorEquipo[eq1].count(eq2)==0){
            partidosPorEquipo[eq1][eq2] = vector<int>(2);
        } 
        if(partidosPorEquipo[eq2].count(eq1)==0){
            partidosPorEquipo[eq2][eq1] = vector<int>(2);
        } 

        partidosPorEquipo[eq1][eq2][0]++;
        partidosPorEquipo[eq2][eq1][1]++;
    }

    return partidosPorEquipo;
}

map<int, vector<int>> generalEquipos(map<int, map<int, vector<int>>>& partidosPorEquipo) {
    map<int, vector<int>> info;

    for(auto pEquipo1 : partidosPorEquipo) {
        int equipo = pEquipo1.first;
        int ganados = 0;
        int perdidos = 0;

        for(auto pEquipo2 : partidosPorEquipo[equipo]) {
            vector<int> resultados = pEquipo2.second;

            ganados += resultados[0];
            perdidos += resultados[1];
        }

        info[equipo] = vector<int>(2);
        info[equipo][0] = ganados;
        info[equipo][1] = perdidos;
    }

    return info;
}

map<int, int> hidratarSistema(
    map<int, map<int, vector<int>>>& partidosPorEquipo,
    map<int, vector<int>>& generalEquipos,
    vector<vector<double>>& c,
    vector<double>& b
) {
    map<int, int> indices;
    int i = 0;

    // armando b
    for(auto pEquipo : generalEquipos) {
        int equipo = pEquipo.first;
        vector<int> info = pEquipo.second;

        b[i] = 1 + ((double)(info[0] - info[1])) / 2;

        // referencia de posiciones, por si se necesita
        indices[equipo] = i++;
    }

    // armando c
    for(auto pEquipo1 : partidosPorEquipo){
        int eq1 = pEquipo1.first;

        for(auto pEquipo2 : partidosPorEquipo[eq1]){
            int eq2 = pEquipo2.first;

            int jugados = -(partidosPorEquipo[eq1][eq2][0] + partidosPorEquipo[eq1][eq2][1]);

            c[indices[eq1]][indices[eq2]] = jugados;
        }
    }

    for(auto pEquipo : generalEquipos) {
        int equipo = pEquipo.first;
        int jugados = 2 + generalEquipos[equipo][0] + generalEquipos[equipo][1];
        c[indices[equipo]][indices[equipo]] = jugados;
    }

    return indices;
}

vector<double> compute(int n, int m, vector<vector<int>>& queries) {
    vector<vector<double>> c(n);
    vector<double> b(n);
    vector<double> r(n);
    set<int> equiposIds;

    for(int i = 0; i < n; i++) {
        c[i] = vector<double>(n);
    }

    auto partidosPorEquipo = calcularPartidos(n, m, queries, equiposIds);
    auto infoGeneralEquipos = generalEquipos(partidosPorEquipo);
    auto indices = hidratarSistema(partidosPorEquipo, infoGeneralEquipos, c, b);

    r = resolver(c, b);

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
