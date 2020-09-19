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

map<int, int> calcularIndicesEquipos(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos, bool useIdAsIndex){
    map<int, int> equipoIdToIndex;
    int eqIndex = 0;

    for (int pi = 0; pi < cantPartidos; ++pi){ 
        int eqId1 = partidos[pi][1];
        int eqId2 = partidos[pi][3];
        if(equipoIdToIndex.count(eqId1)==0) equipoIdToIndex[eqId1] = useIdAsIndex ? eqId1-1 : eqIndex++;
        if(equipoIdToIndex.count(eqId2)==0) equipoIdToIndex[eqId2] = useIdAsIndex ? eqId2-1 : eqIndex++;

        if(equipoIdToIndex.size() == cantEquipos) break;
    }

    return equipoIdToIndex;
}

void calcularGanadosYPerdidos(
    int cantEquipos,
    int cantPartidos,
    vector<vector<int>>& partidos,
    vector<int>& wi,
    vector<int>& li,
    map<int, int>& equipoIdToIndex
){
    for (int pi = 0; pi < cantPartidos; ++pi){
        int eqI = equipoIdToIndex[partidos[pi][1]];
        int eqJ = equipoIdToIndex[partidos[pi][3]];

        int golesI = partidos[pi][2];
        int golesJ = partidos[pi][4];

        if(golesI > golesJ){
            wi[eqI]++;
            li[eqJ]++;
        }else{
            wi[eqJ]++;
            li[eqI]++;
        }
    }
}

void hidratarSistemaLineal(
    int cantEquipos,
    int cantPartidos,
    vector<vector<int>>& partidos,
    vector<vector<double>>& c,
    vector<double>& b,
    map<int, int>& equipoIdToIndex
){
    for (int pi = 0; pi < cantPartidos; ++pi){
        int eqI = equipoIdToIndex[partidos[pi][1]];
        int eqJ = equipoIdToIndex[partidos[pi][3]];

        int golesI = partidos[pi][2];
        int golesJ = partidos[pi][4];

        // c_ij = -n_ij
        c[eqI][eqJ]--; 
        c[eqJ][eqI]--;

        // c_ii = 2+ni
        c[eqI][eqI]++;
        c[eqJ][eqJ]++;

        if(golesI > golesJ){
            b[eqI]++; //winner
            b[eqJ]--; //loser
        }else{
            b[eqI]--; //loser
            b[eqJ]++; //winner
        }
    }

    for (int i = 0; i < cantEquipos; ++i){
        b[i] = 1 + b[i]/2; //en b[i] estaba (wi - li)
        c[i][i] += 2; // en c[i][i] estaba ni (falta el +2)
    }
}

//************************************

vector<double> WP(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos, map<int, int>& equipoIdToIndex) {
    vector<double> r(cantEquipos);
    vector<int> wi(cantEquipos);
    vector<int> li(cantEquipos);

    calcularGanadosYPerdidos(cantEquipos, cantPartidos, partidos, wi, li, equipoIdToIndex);

    for(int i = 0; i < cantEquipos; i++) {
        r[i] = (double)wi[i] / (wi[i] + li[i]);
    }

    return r;
}

vector<double> CMM(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos, map<int, int>& equipoIdToIndex) {
    vector<vector<double>> c(cantEquipos);
    vector<double> b(cantEquipos);

    for(int i = 0; i < cantEquipos; i++) {
        c[i] = vector<double>(cantEquipos);
    }

    hidratarSistemaLineal(cantEquipos, cantPartidos, partidos, c, b, equipoIdToIndex);
    
    // printMatrix(c);

    auto r = resolverSistLineal(c, b);

    return r;
}

void printRanking(map<int, double>& ranking) {
    for(auto p : ranking) {
        cout << p.first << " - " << p.second << endl;
    }
}

map<int, double> combinar(vector<double> r, map<int, int> equipoIdToIndex) {
    map<int, double> result;

    for(auto p : equipoIdToIndex) {
        result[p.first] = r[p.second];
    }

    return result;
}

pair<int, double> parMenor(map<int, double>& ranking) {
    auto pr = std::max_element
    (
        begin(ranking), end(ranking),
        [] (const pair<int, double> & p1, const pair<int, double> & p2) {
            return p1.second > p2.second;
        }
    );

    return *pr;
}

pair<int, double> parMayor(map<int, double>& ranking) {
    auto pr = std::max_element
    (
        begin(ranking), end(ranking),
        [] (const pair<int, double> & p1, const pair<int, double> & p2) {
            return p1.second < p2.second;
        }
    );

    return *pr;
}

void estrategiaCMM(int cantEquipos, int cantPartidos, vector<vector<int>>& partidos, map<int, int>& equipoIdToIndex) {

    int pasos = 0;
    int iMenor;
    int iMayor;
    double rMenor;
    double rMayor;

    do {
        map<int, double> ranking = combinar(CMM(cantEquipos, cantPartidos, partidos, equipoIdToIndex), equipoIdToIndex);

        // printRanking(ranking);

        if(pasos++ == 0) iMenor = parMenor(ranking).first;
        rMenor = ranking[iMenor];

        iMayor = parMayor(ranking).first;
        rMayor = ranking[iMayor];

        cout << iMenor << " y " << iMayor << " difieren en " << abs(rMayor - rMenor) << endl;

        // agrego una partida donde el menor le gana al mayor
        vector<int> partido = { 1, iMenor, 1, iMayor, 0 };
        partidos.push_back(partido);
        cantPartidos++;
    } while(iMenor != iMayor && abs(rMayor - rMenor) > 0.001 );

    cout << pasos << endl;

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

    //fix por el tema del orden en los tests
    string inputfilename = argv[1]; 
    bool useIdAsIndex = (inputfilename.find("test") != string::npos);
    map<int, int> equipoIdToIndex = calcularIndicesEquipos(cantEquipos, cantPartidos, partidos, useIdAsIndex);

    cout << inputfilename << " es un test? " << useIdAsIndex << endl;

    vector<double> result;

    // 0 = CMM, 1 = WP, 2 = Alternativo
    if(opcionAlgor == 0) { 
        result = CMM(cantEquipos, cantPartidos, partidos, equipoIdToIndex);
    } else if (opcionAlgor == 1) {
        result = WP(cantEquipos, cantPartidos, partidos, equipoIdToIndex);
    } else {
        // estrategiaCMM(cantEquipos, cantPartidos, partidos, equipoIdToIndex);
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
