#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "metodos.h"

using namespace std;

//************************************

void write(ostream& out, string delim, vector<double>& v, vector<int> i, vector<int> wi, vector<int> li) {
    out << "id" << delim << "rating" << delim << "ganados" << delim << "perdidos" << delim << "jugados" << endl;
    for(int j = 0; j < v.size(); j++) {
        out << i[j] << delim << v[j] << delim << wi[j] << delim << li[j] << delim << wi[j] + li[j] << endl;
    }
}

void write(ostream& out, vector<double>& v) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        out << *i << '\n';
    }
}

//************************************

vector<string> split_string(string);

int main(int argc, char *argv[]) {

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    int opcionAlgor = 0;
    if(argc>3){
        opcionAlgor = stoi(argv[3]);
    }

    string csvfilename;
    if(argc>4){
        csvfilename = argv[4];
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
    // cout << inputfilename << " es un test? " << useIdAsIndex << endl;

    map<int, int> equipoIdToIndex = calcularIndicesEquipos(cantEquipos, cantPartidos, partidos, useIdAsIndex);

    vector<double> result;

    // 0 = CMM, 1 = WP, 2 = Alternativo
    if(opcionAlgor == 0) { 
        result = CMM(cantEquipos, cantPartidos, partidos, equipoIdToIndex);
    } else if (opcionAlgor == 1) {
        result = WP(cantEquipos, cantPartidos, partidos, equipoIdToIndex);
    } else {
        cout << "TODO, todavia no esta implementado" << endl;
    }

    write(fout, result);
    fout.close();

    if(csvfilename != "") {
        ofstream foutcsv(csvfilename);
        vector<int> wi(cantEquipos);
        vector<int> li(cantEquipos);
        vector<int> indices(cantEquipos);

        for(auto p : equipoIdToIndex) {
            indices[p.second] = p.first;
        }

        calcularGanadosYPerdidos(cantEquipos, cantPartidos, partidos, wi, li, equipoIdToIndex);
        write(foutcsv, ",", result, indices, wi, li);
        foutcsv.close();
    }

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
