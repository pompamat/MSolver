#pragma once
#include "rational.h"
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::max;

const string LONG_LINE = "===============================";
const long long INV = (1ll << 60);

void print_str_matrix(const vector<vector<string>>& V){
    vector<int> col_begin;
    col_begin.push_back(0);
    for(int i = 0; i < V[0].size(); ++i){
        int dl = 0;
        for(int j = 0; j < V.size(); ++j) dl = max(dl, (int) V[j][i].size());
        col_begin.push_back((i != 0) * 1 + dl + col_begin.back());
    }
    for(int i = 0; i < V.size(); ++i){
        for(int j = 0; j < V[0].size(); ++j){
            int t = col_begin[j];
            while(t + V[i][j].size() < col_begin[j + 1]){
                cout << " ";
                t++;
            }
            cout << V[i][j];
        }
        cout << endl;
    }
}

long long score_rational(const rational& r) {
    if (r == ZERO) return 0;
    return 2.5 * r.m + abs(r.l);
}

long long score_monomial(const monomial& m) {
    long long res = 1 + (int) m.variables.size();
    for (const auto& v : m.variables){
        res += v.second * 2;
    }
    return res * score_rational(m.coef);
}

long long score_polynomial(const polynomial& p) {
    long long res = 0;
    for(const monomial& m : p.monomials){
        res += score_monomial(m);
    }
    return res;
}
