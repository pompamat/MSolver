#pragma once
#include <vector>
#include <iostream>

struct monomial {
    std::string token;
    rational amount;

    friend std::ostream& operator <<(std::ostream& str, const monomial &p) {
        if (p.token == "" || p.amount.l == 0) str << p.amount;
        else {
            if (abs(p.amount.l) == 1 && p.amount.m == 1){
                if (p.amount.l < 0) str << "-";
                str << p.token;
            } else str << p.amount << p.token;
        }
        return str;
    }
};

struct polynomial {
    std::vector<monomial> monomials;

    friend std::ostream& operator <<(std::ostream& str, const polynomial &p) {
        bool nic_wpisalem = true;
        for(const monomial& m : p.monomials){
            if (m.amount.l != 0){
                if (m.amount.l < 0) str << m;
                else {
                    if (!nic_wpisalem) str << "+";
                    str << m;
                }
                nic_wpisalem = false;
            }
        }
        if (nic_wpisalem) str << "0";
        return str;
    }
};

polynomial simplify(const polynomial& p){
    polynomial res;
    for(const monomial &m : p.monomials){
        bool succ = false;
        for (monomial& m2 : res.monomials){
            if (m2.token == m.token){
                m2.amount = m2.amount + m.amount;
                succ = true;
                break;
            }
        }
        if (!succ){
            res.monomials.push_back(m);
        }
    }
    return res;
}

polynomial add(const polynomial& a, const polynomial& b){
    polynomial res;
    for(const monomial& m : a.monomials) res.monomials.push_back(m);
    for(const monomial& m : b.monomials) res.monomials.push_back(m);
    return simplify(res);
}

polynomial mul(const rational& r, const polynomial& p){
    polynomial res;
    for(const monomial& m : p.monomials){
        res.monomials.push_back({m.token, m.amount * r});
    }
    return res;
}

rational parse_rational(const std::string &str){
    int idx_of_slash = -1;
    for(int i = 0; i < str.size(); ++i){
        if (str[i] == '/' || str[i] == '\\'){
            if (idx_of_slash != -1){
                std::cerr << "TOKEN ZAWIERA ZBYT DUZO UKOSNIKOW!!" << std::endl;
                exit(136);
            }
            idx_of_slash = i;
        }
    }
    if (idx_of_slash == -1){
        return rational(stoll(str), 1);
    }
    return rational(stoll(str.substr(0, idx_of_slash)), stoll(str.substr(idx_of_slash + 1)));
}

polynomial parse_polynomial(const std::string &str){
    std::vector<int> token_idx;
    for(int i = 1; i < str.size(); ++i){
        if (str[i] == '+' || str[i] == '-'){
            token_idx.push_back(i);
        }
    }
    token_idx.push_back(str.size());
    int prev_end = 0;
    polynomial res;
    for(int x : token_idx){
        int dokiedy = prev_end;
        while(dokiedy < x && ((str[dokiedy] <= '9' && str[dokiedy >= '0']) || str[dokiedy] == '/' || str[dokiedy] == '\\')) dokiedy++;
        rational coef(1, 1);
        if (dokiedy != prev_end) coef = parse_rational(str.substr(prev_end, dokiedy - prev_end));
        if (prev_end != 0 && str[prev_end - 1] == '-') coef.l *= -1;
        res.monomials.push_back({str.substr(dokiedy, x - dokiedy), coef});
        prev_end = x + 1;
    }
    return res;
}
