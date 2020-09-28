#include <vector>
#include <map>
#include "sistema.h"

using namespace std;

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

    auto r = resolverSistLineal(c, b);

    return r;
}