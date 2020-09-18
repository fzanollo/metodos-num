#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

//************************************

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

vector<double> resolver(vector<vector<double>>& c, vector<double>& b) {
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

void hidratarSistema(
    int cantEquipos,
    int cantPartidos,
    vector<vector<int>>& partidos,
    vector<vector<double>>& c,
    vector<double>& b,
    vector<int>& wi,
    vector<int>& li,
    map<int, int> equipoIdToIndex
){
    int i = 0;

    for (int pi = 0; pi < cantPartidos; ++pi){
        int eq1 = partidos[pi][1];
        int goles1 = partidos[pi][2];

        int eq2 = partidos[pi][3];
        int goles2 = partidos[pi][4];

        int iEq1 = equipoIdToIndex[eq1];
        int jEq2 = equipoIdToIndex[eq2];

        // c_ij = -n_ij
        c[iEq1][jEq2]--; 
        c[jEq2][iEq1]--;

        // c_ii = 2+ni
        c[iEq1][iEq1]++;
        c[jEq2][jEq2]++;

        if(goles1 > goles2){
            b[iEq1]++; //winner
            b[jEq2]--; //loser
            wi[iEq1]++;
            li[jEq2]++;
        }else{
            b[iEq1]--; //loser
            b[jEq2]++; //winner
            wi[jEq2]++;
            li[iEq1]++;
        }
    }

    for (int i = 0; i < cantEquipos; ++i){
        b[i] = 1 + b[i]/2; //en b[i] estaba (wi - li)

        c[i][i] += 2; // en c[i][i] estaba ni (falta el +2)
    }
}

void inicializarEstructuras(
    int cantEquipos,
    int cantPartidos,
    vector<vector<int>>& partidos,
    vector<vector<double>>& c,
    vector<double>& b,
    vector<int>& wi,
    vector<int>& li
) {
    for(int i = 0; i < cantEquipos; i++) {
        c[i] = vector<double>(cantEquipos);
    }

    //coso horrible para el tema del orden de los equipos
    map<int, int> equipoIdToIndex;
    int eqIndex = 0;

    for (int pi = 0; pi < cantPartidos; ++pi){ 
        int eqId1 = partidos[pi][1];
        int eqId2 = partidos[pi][3];
        if(equipoIdToIndex.count(eqId1)==0) equipoIdToIndex[eqId1] = eqIndex++;
        if(equipoIdToIndex.count(eqId2)==0) equipoIdToIndex[eqId2] = eqIndex++;
    }

    hidratarSistema(cantEquipos, cantPartidos, partidos, c, b, wi, li, equipoIdToIndex);
}

vector<double> WP(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos) {
    vector<vector<double>> c(cantEquipos);
    vector<double> b(cantEquipos);
    vector<double> r(cantPartidos);
    vector<int> wi(cantEquipos);
    vector<int> li(cantEquipos);

    inicializarEstructuras(cantEquipos, cantPartidos, partidos, c, b, wi, li);

    for(int i = 0; i < cantEquipos; i++) {
        r[i] = wi[i] / (wi[i] + li[i]);
    }

    return r;
}

vector<double> CMM(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos) {
    vector<vector<double>> c(cantEquipos);
    vector<double> b(cantEquipos);
    vector<int> wi(cantEquipos);
    vector<int> li(cantEquipos);

    inicializarEstructuras(cantEquipos, cantPartidos, partidos, c, b, wi, li);

    auto r = resolver(c, b);

    return r;
}



//************************************

vector<string> split_string(string);

void write(vector<double>& v, ofstream& fout) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        fout << *i << '\n';
    }
}

int main(int argc, char *argv[]) {

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    
    int opcionAlgor = 0;
    if(argc>3){
        opcionAlgor = stoi(argv[3]);
    }

    string nm_temp;
    getline(fin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int cantEquipos = stoi(nm[0]);

    int cantPartidos = stoi(nm[1]);


    vector<vector<int>> partidos(cantPartidos);
    for (int i = 0; i < cantPartidos; i++) {
        partidos[i].resize(5);

        for (int j = 0; j < 5; j++) {
            fin >> partidos[i][j];
        }

        fin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<double> result;

    // 0 = CMM, 1 = WP, 2 = Alternativo
    if(opcionAlgor == 0) { 
        result = CMM(cantEquipos, cantPartidos, partidos);
    } else if (opcionAlgor == 1) {
        result = WP(cantEquipos, cantPartidos, partidos);
    } else {
        cout << "TODO, todavia no esta implementado" << endl;
    }

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
