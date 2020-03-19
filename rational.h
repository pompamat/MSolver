#pragma once
#include <iostream>

long long gcdabs(long long a, long long b){
    return (b == 0 ? a : gcdabs(b, a % b));
}

long long gcd(long long a, long long b){
    return gcdabs(abs(a), abs(b));
}

struct rational {
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
};

rational abs(const rational& a){
    if (a.l >= 0) return a;
    return rational(-a.l, a.m);
}
