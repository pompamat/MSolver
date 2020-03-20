#pragma once
#include <iostream>

long long gcdabs(long long a, long long b){
    return (b == 0 ? a : gcdabs(b, a % b));
}

long long gcd(long long a, long long b){
    return gcdabs(abs(a), abs(b));
}

const struct rational {
    long long l, m;

    rational(long long a, long long b){
        long long d = gcd(a, b);
        if (b < 0){
            a *= -1;
            b *= -1;
        }
        if (b == 0) {
            exit(155);
        }
        l = a;
        m = b;
        if (d != 1){
            l /= d;
            m /= d;
        }
    }

    friend std::ostream& operator <<(std::ostream& str, const rational& p) {
        str << p.l;
        if (p.m != 1){
            str << "/" << p.m;
        }
        return str;
    }

    rational operator+ (const rational& b) const {
        return rational(l * b.m + b.l * m, m * b.m);
    }

    rational operator- () const {
        return rational(-l, m);
    }

    rational operator* (const rational& b) const {
        return rational(l * b.l, m * b.m);
    }

    rational operator! () const {
        if (l == 0){
            std::cerr << "NIE MOGE ODWROCIC ZERA!" << std::endl;
            exit(117);
        }
        return rational(m, l);
    }

    rational operator/ (const rational& b) const {
        return (*this) * !b;
    }

    bool operator== (const rational& o) const {
        return l == o.l && m == o.m;
    }

    bool operator!= (const rational& o) const {
        return !(*this == o);
    }

    bool operator< (const rational& o) const {
        return l * o.m < m * o.l;
    }
} ZERO(0, 1), ONE(1, 1);

rational abs(const rational& a){
    if (a.l >= 0) return a;
    return rational(-a.l, a.m);
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
