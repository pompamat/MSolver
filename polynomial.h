#pragma once
#include "rational.h"
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <cctype>

using std::vector;
using std::pair;
using std::ostream;
using std::sort;
using std::string;

struct monomial { // struct representing single monomial, eg. -2/3pq^2
    vector<pair<char, int>> variables; //with positive integer multiplicity
    rational coef;

    monomial(vector<pair<char, int>> variables_, const rational& r) : coef(r) {
        if (r == ZERO){
            variables = {};
        } else {
            sort(variables_.begin(), variables_.end());
            char last_variable = '0';

            for (const auto& variable : variables_){
                assert(islower(variable.first));
                if (last_variable == variable.first){
                    variables.back().second += variable.second;
                } else {
                    variables.push_back(variable);
                }
                last_variable = variable.first;
            }
        }
    }

    friend ostream& operator <<(ostream& str, const monomial &p) {
        if (p.variables.size() == 0) {
            str << p.coef;
        } else {
            if (abs(p.coef) == ONE) {
                if (p.coef < ZERO) str << "-";
            } else str << p.coef;
            for (const auto& variable : p.variables) {
                str << variable.first;
                if (variable.second > 1){
                    str << "^" << variable.second;
                }
            }
        }
        return str;
    }

    bool is_compatible(const monomial& o) const {
        if (variables.size() != o.variables.size()) return false;
        for (int i = 0; i < variables.size(); ++i){
            if (variables[i] != o.variables[i]) return false;
        }
        return true;
    }

    monomial operator* (const monomial& o) const {
        vector<pair<char, int>> variables_(variables);
        copy(o.variables.begin(), o.variables.end(), back_inserter(variables_));
        return monomial(variables_, coef * o.coef);
    }

    bool operator< (const monomial& o) const {
        int t = 0;
        while(t < variables.size()){
            if (t == o.variables.size()) return false;
            if (variables[t] != o.variables[t]) return variables[t] < o.variables[t];
            t++;
        }
        if (variables.size() < o.variables.size()) return true;
        return coef < o.coef; //identical up to factor
    }
};


struct polynomial {
    vector<monomial> monomials;

    polynomial(vector<monomial> monomials_) {
        sort(monomials_.begin(), monomials_.end());
        for (const auto& m : monomials_){
            if (m.coef == ZERO) continue;
            if (monomials.size() != 0 && monomials.back().is_compatible(m)){
                monomials.back().coef = monomials.back().coef + m.coef;
            } else {
                monomials.push_back(m);
            }
        }
    }

    polynomial(const rational& r) : polynomial({{{}, r}}) {}

    friend ostream& operator <<(ostream& str, const polynomial &p) {
        bool first_token = true;
        for(const monomial& m : p.monomials){
            if (m.coef != ZERO){
                if (m.coef < ZERO) str << m;
                else {
                    if (!first_token) str << "+";
                    str << m;
                }
                first_token = false;
            }
        }
        if (first_token) str << "0";
        return str;
    }

    polynomial operator+ (const polynomial& o){
        vector<monomial> res_monomials(monomials);
        copy(o.monomials.begin(), o.monomials.end(), back_inserter(res_monomials));
        return polynomial(res_monomials);
    }

    polynomial operator* (const rational& r){
        vector<monomial> res_monomials;
        for(const monomial& m : monomials){
            res_monomials.push_back(monomial(m.variables, m.coef * r));
        }
        return polynomial(res_monomials);
    }

    polynomial operator* (const polynomial& o){
        vector<monomial> res_monomials;
        for(const monomial& m : monomials) {
            for (const monomial& m2 : o.monomials){
                res_monomials.push_back(m * m2);
            }
        }
        return polynomial(res_monomials);
    }

    void operator+= (const polynomial& o) {
        polynomial sum = *this + o;
        monomials = ::std::move(sum.monomials);
    }
};

bool is_rational_char(char c){
    if (c == '+' || c == '-') return true;
    if (isdigit(c)) return true;
    return c == '/' || c == '\\';
}

monomial parse_monomial(const string &str){
    int iter = 0;
    while(iter < str.size() && is_rational_char(str[iter])) iter++;
    rational coef = ONE;
    if (iter == 1 && str[0] == '-') coef = -ONE;
    if (iter > 1 || (iter == 1 && isdigit(str[0]))) coef = parse_rational(str.substr(0, iter));
    vector<pair<char, int>> variables;
    while(iter < str.size()){
        char var_name = str[iter];
        iter++;
        int power = 1;
        if (iter < str.size() && str[iter] == '^'){
            iter++;
            int pow_idx = iter;
            while(pow_idx < str.size() && (isdigit(str[pow_idx]))) pow_idx++;
            power = stoi(str.substr(iter, pow_idx - iter));
            iter = pow_idx;
        }
        variables.push_back({var_name, power});
    }
    return monomial(variables, coef);
}

polynomial parse_polynomial(const string &str){
    std::vector<int> token_idx;
    for(int i = 1; i < str.size(); ++i){
        if (str[i] == '+' || str[i] == '-'){
            token_idx.push_back(i);
        }
    }
    token_idx.push_back(str.size());
    int prev_end = 0;
    vector<monomial> res_monomials;
    for(int x : token_idx){
        res_monomials.push_back(parse_monomial(str.substr(prev_end, x - prev_end)));
        prev_end = x;
    }
    return polynomial(res_monomials);
}
