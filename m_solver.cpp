#include<bits/stdc++.h>
#define FOR(i,a,b) for(int i=a;i<b;++i)
#define FORD(i,a,b) for(int i=a;i>=b;--i)
#define PB push_back
#define FI first
#define SE second
#define umap unordered_map
#define uset unordered_set
#define vi vector<int>
#define vvi vector<vi>
#define pii pair<int, int>
#define ALL(X) (X).begin(),(X).end()
using namespace std;
using ll = long long;
using ld = long double;

ll gcdabs(ll a, ll b){
    return (b == 0 ? a : gcdabs(b, a % b));
}

ll gcd(ll a, ll b){
    return gcdabs(abs(a), abs(b));
}

struct rational {
    ll l, m;

    rational(ll a, ll b){
        ll d = gcd(a, b);
        if (b < 0){
            a *= -1;
            b *= -1;
        }
        if (b == 0){
            cerr << "NIE MOZNA DZIELIC PRZEZ 0" << endl;
            exit(155);
        }
        l = a;
        m = b;
        if (d != 1){
            l /= d;
            m /= d;
        }
    }

    friend ostream& operator <<(ostream& str, const rational &p) {
        str << p.l;
        if (p.m != 1){
            str << "/" << p.m;
        }
        return str;
    }
};

rational add(const rational& a, const rational& b){
    return rational(b.m * a.l + b.l * a.m, a.m * b.m);
}

rational neg(const rational& a){
    return rational(-a.l, a.m);
}

rational sub(const rational& a, const rational& b){
    return add(a, neg(b));
}

rational mul(const rational& a, const rational& b){
    return rational(a.l * b.l, a.m * b.m);
}

rational inv(const rational& a){
    if (a.l == 0){
        cerr << "NIE MOGE ODWROCIC ZERA!" << endl;
        exit(117);
    }
    return rational(a.m, a.l);
}

rational abss(const rational& a){
    if (a.l >= 0) return a;
    return rational(-a.l, a.m);
}

rational div(const rational& a, const rational& b){
    return mul(a, inv(b));
}

struct monomial {
    string token;
    rational amount;

    friend ostream& operator <<(ostream& str, const monomial &p) {
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
    vector<monomial> monomials;

    friend ostream& operator <<(ostream& str, const polynomial &p) {
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
                m2.amount = add(m2.amount, m.amount);
                succ = true;
                break;
            }
        }
        if (!succ){
            res.monomials.PB(m);
        }
    }
    return res;
}

polynomial add(const polynomial& a, const polynomial& b){
    polynomial res;
    for(const monomial& m : a.monomials) res.monomials.PB(m);
    for(const monomial& m : b.monomials) res.monomials.PB(m);
    return simplify(res);
}

polynomial mul(const rational& r, const polynomial& p){
    polynomial res;
    for(const monomial& m : p.monomials){
        res.monomials.PB({m.token, mul(m.amount, r)});
    }
    return res;
}

rational parse_rational(const string &str){
    int idx_of_slash = -1;
    FOR(i,0,str.size()){
        if (str[i] == '/' || str[i] == '\\'){
            if (idx_of_slash != -1){
                cerr << "TOKEN ZAWIERA ZBYT DUZO UKOSNIKOW!!" << endl;
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

polynomial parse_polynomial(const string &str){
    vector<int> token_idx;
    FOR(i,1,str.size()){
        if (str[i] == '+' || str[i] == '-'){
            token_idx.PB(i);
        }
    }
    token_idx.PB(str.size());
    int prev_end = 0;
    polynomial res;
    for(int x : token_idx){
        int dokiedy = prev_end;
        while(dokiedy < x && ((str[dokiedy] <= '9' && str[dokiedy >= '0']) || str[dokiedy] == '/' || str[dokiedy] == '\\')) dokiedy++;
        rational coef(1, 1);
        if (dokiedy != prev_end) coef = parse_rational(str.substr(prev_end, dokiedy - prev_end));
        if (prev_end != 0 && str[prev_end - 1] == '-') coef.l *= -1;
        res.monomials.PB({str.substr(dokiedy, x - dokiedy), coef});
        prev_end = x + 1;
    }
    return res;
}

struct task {
    int rown, niewiad, dol;
    vector<vector<rational>> M;
    vector<vector<polynomial>> D;

    void print(){
        int tot = niewiad + dol;
        vector<vector<string>> V(rown, vector<string>(tot));
        FOR(i,0,rown){
            FOR(j,0,niewiad){
                ostringstream oss;
                oss << M[i][j];
                V[i][j] = oss.str();
            }
            FOR(j,0,dol){
                ostringstream oss;
                oss << D[i][j];
                V[i][niewiad + j] = oss.str();
            }
        }
        vector<int> najwczesniej_zaczecie;
        najwczesniej_zaczecie.PB(0);
        FOR(i,0,tot){
            int dl = 0;
            FOR(j,0,rown) dl = max(dl, (int) V[j][i].size());
            najwczesniej_zaczecie.PB((i == niewiad ? 2 : 1) + dl + najwczesniej_zaczecie.back());
        }
        FOR(i,0,rown){
            FOR(j,0,tot){
                int t = najwczesniej_zaczecie[j];
                while(t + V[i][j].size() < najwczesniej_zaczecie[j + 1]){
                    cout << " ";
                    t++;
                }
                cout << V[i][j];
            }
            cout << endl;
        }
    }
};

void read_task(task& t){
    assert(t.rown >= 1 && t.rown <= 20);
    assert(t.niewiad >= 1 && t.niewiad <= 20);
    assert(t.dol >= 0 && t.dol <= 20);
    t.M.resize(t.rown);
    t.D.resize(t.rown);
    FOR(i,0,t.rown){
        string token;
        FOR(j,0,t.niewiad){
            cin >> token;
            t.M[i].PB(parse_rational(token));
        }
        FOR(j,0,t.dol){
            cin >> token;
            t.D[i].PB(parse_polynomial(token));
        }
    }
}

const ll INV = (1ll << 60);

ll score_rationala(const rational& r){
    if (r.l == 0) return INV;
    return 5 * r.m + 2 * abs(r.l);
}

const string LONG_LINE = "===============================";

void solve_task(task& t){
    t.print();
    cout << LONG_LINE << endl;
    vector<pii> zerowanko;
    int acc_rows = 0;
    FOR(i,0,t.niewiad){
        //znajdz wiersz-kandydat do zerowania kolumny
        ll b_score = INV;
        int idx = -1;
        FOR(j,acc_rows,t.rown){
            ll n_score = score_rationala(t.M[j][i]);
            if (b_score > n_score){
                b_score = n_score;
                idx = j;
            }
        }
        if (idx == -1){
            cout << "Nie mam czym wyzerowac kolumny " << i + 1 << endl;
            continue;
        }
        FOR(j,0,t.rown){
            if (j == idx) continue;
            if (t.M[j][i].l != 0){
                rational coef = neg(div(t.M[j][i], t.M[idx][i]));
                cout << "W" << j + 1 << " " << (coef.l < 0 ? "-= " : "+= ") << abss(coef) << " * W" << idx + 1 << endl;
                FOR(zz,0,t.niewiad){
                    t.M[j][zz] = add(t.M[j][zz], mul(coef, t.M[idx][zz]));
                }
                FOR(zz,0,t.dol){
                    t.D[j][zz] = add(t.D[j][zz], mul(coef, t.D[idx][zz]));
                }
            }
        }
        if (idx != acc_rows){
            cout << "Zamiana miejscami W" << acc_rows + 1 << " i W" << idx + 1 << endl;
            FOR(zz,0,t.niewiad){
                swap(t.M[idx][zz], t.M[acc_rows][zz]);
            }
            FOR(zz,0,t.dol){
                swap(t.D[idx][zz], t.D[acc_rows][zz]);
            }
        }
        zerowanko.PB({acc_rows, i});
        acc_rows++;
        t.print();
        cout << LONG_LINE << endl;
    }
    cout << LONG_LINE << endl;
    for(const pii& p : zerowanko){
        if (t.M[p.FI][p.SE].l != 1 || t.M[p.FI][p.SE].m != 1){
            rational coef = inv(t.M[p.FI][p.SE]);
            cout << "W" << p.FI << " *= " << coef << endl;
            FOR(zz,0,t.niewiad){
                t.M[p.FI][zz] = mul(coef, t.M[p.FI][zz]);
            }
            FOR(zz,0,t.dol){
                t.D[p.FI][zz] = mul(coef, t.D[p.FI][zz]);
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
