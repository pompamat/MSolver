#include "rational.h"
#include "polynomial.h"
#include "utils.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

struct task {
    int rown, niewiad, dol;
    vector<vector<rational>> M;
    vector<vector<polynomial>> D;

    void print(){
        int tot = niewiad + dol + 1;
        vector<vector<string>> V(rown, vector<string>(tot));
        for(int i = 0; i < rown; ++i){
            for(int j = 0; j < niewiad; ++j){
                ostringstream oss;
                oss << M[i][j];
                V[i][j] = oss.str();
            }
            V[i][niewiad] = "";
            for(int j = 0; j < dol; ++j){
                ostringstream oss;
                oss << D[i][j];
                V[i][niewiad + j + 1] = oss.str();
            }
        }
        print_str_matrix(V);
    }
};

void read_task(task& t){
    assert(t.rown >= 1 && t.rown <= 20);
    assert(t.niewiad >= 1 && t.niewiad <= 20);
    assert(t.dol >= 0 && t.dol <= 20);
    t.M.resize(t.rown);
    t.D.resize(t.rown);
    for(int i = 0; i < t.rown; ++i){
        string token;
        for(int j = 0; j < t.niewiad; ++j){
            cin >> token;
            t.M[i].push_back(parse_rational(token));
        }
        for(int j = 0; j < t.dol; ++j){
            cin >> token;
            t.D[i].push_back(parse_polynomial(token));
        }
    }
}

const long long INV = (1ll << 60);

long long score_rationala(const rational& r){
    if (r.l == 0) return INV;
    return 5 * r.m + 2 * abs(r.l);
}

const string LONG_LINE = "===============================";

void solve_task(task& t){
    t.print();
    cout << LONG_LINE << endl;
    vector<pair<int, int>> zerowanko;
    int acc_rows = 0;
    for(int i = 0; i < t.niewiad; ++i){
        //znajdz wiersz-kandydat do zerowania kolumny
        long long b_score = INV;
        int idx = -1;
        for(int j = acc_rows; j < t.rown; ++j){
            long long n_score = score_rationala(t.M[j][i]);
            if (b_score > n_score){
                b_score = n_score;
                idx = j;
            }
        }
        if (idx == -1){
            cout << "Nie mam czym wyzerowac kolumny " << i + 1 << endl;
            continue;
        }
        for(int j = 0; j < t.rown; ++j){
            if (j == idx) continue;
            if (t.M[j][i].l != 0){
                rational coef = -(t.M[j][i] / t.M[idx][i]);
                cout << "W" << j + 1 << " " << (coef.l < 0 ? "-= " : "+= ") << abs(coef) << " * W" << idx + 1 << endl;
                for(int zz = 0; zz < t.niewiad; ++zz){
                    t.M[j][zz] = t.M[j][zz] + coef * t.M[idx][zz];
                }
                for(int zz = 0; zz < t.dol; ++zz){
                    t.D[j][zz] += t.D[idx][zz] * coef;
                }
            }
        }
        if (idx != acc_rows){
            cout << "Zamiana miejscami W" << acc_rows + 1 << " i W" << idx + 1 << endl;
            for(int zz = 0; zz < t.niewiad; ++zz){
                swap(t.M[idx][zz], t.M[acc_rows][zz]);
            }
            for(int zz = 0; zz < t.dol; ++zz){
                swap(t.D[idx][zz], t.D[acc_rows][zz]);
            }
        }
        zerowanko.push_back({acc_rows, i});
        acc_rows++;
        t.print();
        cout << LONG_LINE << endl;
    }
    cout << LONG_LINE << endl;
    for(const pair<int, int>& p : zerowanko){
        if (t.M[p.first][p.second].l != 1 || t.M[p.first][p.second].m != 1){
            rational coef = !t.M[p.first][p.second];
            cout << "W" << p.first << " *= " << coef << endl;
            for(int zz = 0; zz < t.niewiad; ++zz){
                t.M[p.first][zz] = coef * t.M[p.first][zz];
            }
            for(int zz = 0; zz < t.dol; ++zz){
                t.D[p.first][zz] = t.D[p.first][zz] * coef;
            }
        }
    }
    t.print();
}

int main () {
    task t;
    cout << "Podaj liczbe rownan: ";
    cin >> t.rown;
    cout << "Podaj liczbe niewiadomych: ";
    cin >> t.niewiad;
    cout << "Podaj liczbe dolaczonych kolumn: ";
    cin >> t.dol;
    cout << "Podaj macierz (l.wierszy=" << t.rown << ", l.kol.=" << t.niewiad + t.dol << "):" << endl;

    read_task(t);
    cout << LONG_LINE << endl;
    solve_task(t);

}
