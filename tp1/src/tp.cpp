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

vector<vector<double>> triangular(int n, int m, vector<vector<double>>& C){
    int h = 0; //pivot row
    int k = 0; //pivot column

    while(h < m && k < n) {
        if(C[h][k] == 0) cout << "Can't solve this system! There's a 0 in (" << h << ", " << k << ")" << endl;

        for (int i = h+1; i < m; ++i){
            double f = C[i][k] / C[h][k];
            C[i][k] = 0;

            for (int j = k+1; j < n; ++j){
                C[i][j] = C[i][j] - C[h][j]*f;
            }
        }

        h++;
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

map<int, int> hidratarSistema(int n, int m, vector<vector<int>>& queries, 
    vector<vector<double>>& c,
    vector<double>& b
){
    map<int, int> equipoIdToIndex;    
    int i = 0;

    for (int pi = 0; pi < m; ++pi){
        int eq1 = queries[pi][1];
        int eq2 = queries[pi][3];

        if(equipoIdToIndex.count(eq1)==0) equipoIdToIndex[eq1] = i++;
        if(equipoIdToIndex.count(eq2)==0) equipoIdToIndex[eq2] = i++;

        int iEq1 = equipoIdToIndex[eq1];
        int iEq2 = equipoIdToIndex[eq2];

        // c_ij = -n_ij
        c[iEq1][iEq2]--; 
        c[iEq2][iEq1]--;

        // c_ii = 2+ni
        c[iEq1][iEq1]++;
        c[iEq2][iEq2]++;
        
        b[iEq1]++; //winner
        b[iEq2]--; //loser
    }

    for (int i = 0; i < n; ++i){
        b[i] = 1 + b[i]/2; //en b[i] estaba (wi - li)

        c[i][i] += 2; // en c[i][i] estaba ni (falta el +2)
    }

    return equipoIdToIndex;
}

vector<double> compute(int n, int m, vector<vector<int>>& queries) {
    map<int, int> equipoIdToIndex;
    
    vector<vector<double>> c(n);
    vector<double> b(n);

    for(int i = 0; i < n; i++) {
        c[i] = vector<double>(n);
    }

    equipoIdToIndex = hidratarSistema(n, m, queries, c, b);

    auto r = resolver(c, b);

    return r;
}

void write(vector<double>& v, ofstream& fout) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        fout << *i << '\n';
    }
}

int main(int argc, char *argv[]) {

    string inputFile = argv[1];
    auto output = inputFile.replace(inputFile.rfind('.'), inputFile.size(),".out");

    ifstream fin(argv[1]);
    ofstream fout(output);

    string nm_temp;
    getline(fin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);


    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(5);

        for (int j = 0; j < 5; j++) {
            fin >> queries[i][j];
        }

        fin.ignore(numeric_limits<streamsize>::max(), '\n');
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
