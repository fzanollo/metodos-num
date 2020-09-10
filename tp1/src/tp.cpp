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
#include <iterator>
#include <sstream>

using namespace std;

vector<long> compute(int n, int m, vector<vector<int>>& queries) {
    // TODO: armar C, b; resolver C * r = b; devolver r.
    vector<long> r(m);
    for(int i = 0; i < m; i++) {
        r[i] = queries[i][1];
    }
    return r;
}

void write(vector<int>& v, fstream& fout) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        fout << *i ;
        fout<<",";
    }
    fout<< '\n';
}

void write_csv(vector<vector<int>>& v, string path){
    fstream fout;
    fout.open(path, ios::out | ios::app);

    int cant_filas = v.size();
    for(int i=0; i<cant_filas;i++){
        write(v[i], fout);
    }
}

int read_csv(string path, int filas, int columnas) {
    ifstream read_csv;
    // Open an existing file
    read_csv.open(path, ios::in);
    if(!read_csv)
        return -1;
    string line, cell;
    string fileline, filecell;
    int prevNoOfCols = 0, noOfRows = 0, noOfCols = 0;
    while (getline(read_csv, fileline)) {
        noOfCols = 0;
        stringstream linestream(fileline);
        while (getline(linestream, filecell, ',')) {
            stod(filecell);
            noOfCols++;
        }
        if (noOfRows++ == 0)
            prevNoOfCols = noOfCols;
        if (prevNoOfCols != noOfCols)
            return -1;
    }
    read_csv.close();
    //aca debería pasarse noOfrow y noOfCols, que definen el tamaño de la matriz
    int  parsedCsv[2][6];

    read_csv.open(path,ios::in);
    int i = 0;
    while (getline(read_csv, fileline)) {
        int j = 0;
        stringstream linestream(fileline);
        while (getline(linestream, filecell, ',')) {
            parsedCsv[i][j] = stod(filecell);
            j++;
        }
        i++;
    }
    cout<<parsedCsv;
    return 0;
}

int main(int num_arguments, char* argv[]) {
    // TODO: levantar argv del enunciado
    string medida = argv[1]; //representa a o
    string salidaOentrada = argv[2]; //representa a nombre del archivo
    vector<vector<int>> vec;
    vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(3);
    vec.push_back(vec2);
  //  write_csv(vec, salidaOentrada);

    read_csv(salidaOentrada, 1,2);
    return 0;
}
