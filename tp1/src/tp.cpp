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

vector<long> compute(int n, int m, vector<vector<int>>& queries) {
    // TODO: armar C, b; resolver C * r = b; devolver r.
    vector<long> r(m);
    for(int i = 0; i < m; i++) {
        r[i] = queries[i][1];
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
