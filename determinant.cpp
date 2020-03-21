#include "polynomial.h"
#include "utils.h"
#include <vector>
#include <sstream>

using std::vector;
using namespace std;

struct determinant {
    vector<vector<polynomial>> elems;
    string name;

    void print() const {
        vector<vector<string>> V(elems.size(), vector<string>(elems.size() + 2));
        for(int i = 0; i < elems.size(); ++i){
            for(int j = 0; j < elems.size(); ++j){
                ostringstream oss;
                oss << elems[i][j];
                V[i][j + 1] = oss.str();
            }
            V[i][0] = V[i][elems.size() + 1] = "|";
        }
        V[(elems.size() - 1) / 2][0] = name + " = |";
        print_str_matrix(V);
    }
};

polynomial compute_determinant(const determinant& det);
polynomial laplace_expansion(const determinant& det, char orientation, int idx);

// zero as much elements along orientation with element [rx][cx]
vector<pair<int, rational>> compute_gauss_reduction(const determinant& det, char orientation, int rx, int cx) {
    if (det.elems[rx][cx].monomials.size() == 0) return {};
    int r = (orientation == 'r' ? rx : -1);
    int c = (orientation == 'c' ? cx : -1);
    vector<pair<int, rational>> res;

    for(int i = 0; i < det.elems.size(); ++i){
        if (orientation == 'c') r++;
        if (orientation == 'r') c++;
        if (r == rx && c == cx) continue;
        if (det.elems[r][c].monomials.size() == 0) {
            res.push_back({(orientation == 'r' ? c : r), ZERO});
            continue;
        }
        auto elem_res = is_rational_multiple(det.elems[r][c], det.elems[rx][cx]);
        if (elem_res.first){
            res.push_back({(orientation == 'r' ? c : r), elem_res.second});
        }
    }
    return res;
}

long long score_sarus(const determinant& det){
    long long res = 0;
    for(int i = 0; i < det.elems.size(); ++i){
        for(int j = 0; j < det.elems.size(); ++j){
            long long p_score = score_polynomial(det.elems[i][j]);
            res += p_score;
        }
    }
    return res;
}

polynomial compute_with_sarus(const determinant& det) {
    cout << "Licze " << det.name << " z Sarusa." << endl;
    polynomial positive[3] = {
        det.elems[0][0] * det.elems[1][1] * det.elems[2][2],
        det.elems[0][1] * det.elems[1][2] * det.elems[2][0],
        det.elems[0][2] * det.elems[1][0] * det.elems[2][1]
    };
    polynomial negative[3] = {
        det.elems[0][2] * det.elems[1][1] * det.elems[2][0],
        det.elems[0][1] * det.elems[1][0] * det.elems[2][2],
        det.elems[0][0] * det.elems[1][2] * det.elems[2][1]
    };
    polynomial accumulator = positive[0];
    cout << det.name << " = (" << positive[0] << ")" << endl;
    for(int i = 1; i < 3; ++i){
        cout << "    + (" << positive[i] << ") " << endl;
        accumulator = accumulator + positive[i];
    }
    for(int i = 0; i < 3; ++i){
        cout << "    - (" << negative[i] << ") " << endl;
        accumulator = accumulator - negative[i];
    }
    cout << "    = " << accumulator <<  endl << LONG_LINE << endl;
    return accumulator;
}

polynomial compute_determinant(const determinant& det) {
    cout << "Liczymy wyznacznik:" << endl;
    det.print();
    cout << LONG_LINE << endl;
    // just some trivial, low-dimensional cases
    if(det.elems.size() == 0) {
        //0x0 matrix?? sum of one element, which is an empty product
        cout << det.name << " = " << ONE << endl << LONG_LINE << endl;
        return polynomial(ONE);
    }
    if (det.elems.size() == 1) {
        cout << det.name << " = " << det.elems[0][0] << endl << LONG_LINE << endl;
        return det.elems[0][0];
    }
    if (det.elems.size() == 2){
        polynomial res = det.elems[0][0] * det.elems[1][1] - det.elems[0][1] * det.elems[1][0];
        cout << det.name << " = " << res << endl << LONG_LINE << endl;
        return res;
    }
    //actual mathematics

    //maybe a row/col made of zeros?
    for(int i = 0; i < det.elems.size(); ++i){
        bool all_zeros_r = true, all_zeros_c = true;
        for(int o = 0; o < det.elems.size(); ++o){ //orthogonal direction
            if (det.elems[i][o].monomials.size() != 0) all_zeros_r = false;
            if (det.elems[o][i].monomials.size() != 0) all_zeros_c = false;
            if (!all_zeros_c && !all_zeros_r) break;
        }
        if (all_zeros_c){
            cout << "Kolumna nr " << i + 1 << " zawiera same zera, zatem " << det.name << " = 0\n";
            return polynomial(ZERO);
        }
        if (all_zeros_r){
            cout << "Wiersz nr " << i + 1 << " zawiera same zera, zatem " << det.name << " = 0\n";
            return polynomial(ZERO);
        }
    }

    //find best gaussian reduction
    char orientation = 'r';
    int r_idx = 0;
    int c_idx = 0;
    long long best_score = INV;
    vector<pair<int, rational>> reduction;
    for(int i = 0; i < det.elems.size(); ++i){
        for(int j = 0; j < det.elems.size(); ++j){
            auto r_reduction = compute_gauss_reduction(det, 'r', i, j);
            auto c_reduction = compute_gauss_reduction(det, 'c', i, j);

            if (r_reduction.size() >= reduction.size()){
                long long reduction_score = 0;
                for (const auto& acc_rows : r_reduction) {
                    reduction_score += score_rational(acc_rows.second);
                }
                long long col_score = 0;
                for (int s = 0; s < det.elems.size(); ++s){
                    col_score += score_polynomial(det.elems[s][j]);
                }
                reduction_score *= col_score;
                if (reduction_score < best_score || r_reduction.size() > reduction.size()) {
                    best_score = reduction_score;
                    orientation = 'r';
                    reduction = r_reduction;
                    r_idx = i;
                    c_idx = j;
                }
            }

            if (c_reduction.size() >= reduction.size()) {
                long long reduction_score = 0;
                for (const auto& acc_cols : c_reduction) {
                    reduction_score += score_rational(acc_cols.second);
                }
                long long row_score = 0;
                for (int s = 0; s < det.elems.size(); ++s) {
                    row_score += score_polynomial(det.elems[i][s]);
                }
                reduction_score *= row_score;
                if (reduction_score < best_score || c_reduction.size() > reduction.size()) {
                    best_score = reduction_score;
                    orientation = 'c';
                    reduction = c_reduction;
                    r_idx = i;
                    c_idx = j;
                }
            }
        }
    }
    if (det.elems.size() == 3){
        // maybe use sarus?
        if (reduction.size() < 2 || score_sarus(det) < best_score){
            return compute_with_sarus(det);
        }
    }
    //apply gaussian reduction
    bool all_zeros = true;
    for(const auto& r : reduction) {
        if (r.second != ZERO) {
            all_zeros = false;
            break;
        }
    }
    if (reduction.size() > 0 && !all_zeros) {
        string op_type = (orientation == 'c' ? "W" : "K");
        int op_idx = (orientation == 'c' ? r_idx : c_idx);
        determinant result(det);
        for(const auto& r : reduction){
            if (r.second == ZERO) continue;
            cout << op_type << r.first + 1 << " " << (r.second < ZERO ? "+" : "-") << "= " << abs(r.second) << " * " << op_type << op_idx + 1 << endl;
            if (orientation == 'c'){
                for(int s = 0; s < det.elems.size(); ++s){
                    result.elems[r.first][s] = result.elems[r.first][s] - result.elems[r_idx][s] * r.second;
                }
            } else {
                for(int s = 0; s < det.elems.size(); ++s){
                    result.elems[s][r.first] = result.elems [s][r.first] - result.elems[s][c_idx] * r.second;
                }
            }
        }
        cout << LONG_LINE << endl;
        result.print();
        cout << LONG_LINE << endl;
        return laplace_expansion(result, orientation, r_idx + c_idx - op_idx);
    }
    return laplace_expansion(det, orientation, (orientation == 'r' ? r_idx : c_idx));
}

// todo: make this not global
int cofactor_idx = 1;

polynomial laplace_expansion(const determinant& det, char orientation, int idx) {
    cout << "Licze rozwiniecie Laplace'a wzgledem " << idx + 1 << (orientation == 'r' ? " wiersza" : " kolumny") << endl;
    int sign = (idx % 2 == 0 ? 1 : -1);
    int r = (orientation == 'r' ? idx : 0);
    int c = (orientation == 'c' ? idx : 0);
    cout << det.name << " = ";
    vector<pair<determinant, polynomial>> cofactors;
    bool was_printed = false;
    for(int i = 0; i < det.elems.size(); ++i){
        if (det.elems[r][c].monomials.size() != 0){
            polynomial factor = det.elems[r][c];
            if (was_printed){
                cout << "    + ";
            }
            if (sign == -1){
                cout << "(-1) * ";
                factor = factor * (-ONE);
            }
            else cout << "1 * ";
            determinant minor;
            minor.elems.resize(det.elems.size() - 1);
            int rm = 0;
            for(int rmx = 0; rmx < det.elems.size(); ++rmx){
                if (rmx == r) continue;
                for(int cmx = 0; cmx < det.elems.size(); ++cmx){
                    if (cmx == c) continue;
                    minor.elems[rm].push_back(det.elems[rmx][cmx]);
                }
                rm++;
            }
            minor.name = "M_" + to_string(cofactor_idx);
            cofactor_idx++;
            cout << "(" << det.elems[r][c] << ") * " << minor.name << endl;
            cofactors.push_back({minor, factor});
            was_printed = true;
        }
        if (orientation == 'r') c++;
        else r++;
        sign *= (-1);
    }
    polynomial total(ZERO);
    if (cofactors.size() == 0){
        cout << "0" << endl;
        return total;
    }
    cout << "Gdzie:" << endl;
    for (int i = 0; i < cofactors.size(); ++i){
        cofactors[i].first.print();
        cout << LONG_LINE << endl;
    }
    for (const auto& cofactor : cofactors){
        total += cofactor.second * compute_determinant(cofactor.first);
    }
    cout << "Wyznacznik " << det.name << " = " << total << endl << LONG_LINE << endl;
    return total;
}

int main (){
    cout << "Podaj rozmiar wyznacznika: ";
    int n;
    cin >> n;
    determinant det;
    det.elems.resize(n);
    det.name = "D";
    cout << "Podaj elementy wyznacznika (" << n << "x" << n << "):\n";
    for(int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            string s;
            cin >> s;
            det.elems[i].push_back(parse_polynomial(s));
        }
    }
    cout << LONG_LINE << endl;
    compute_determinant(det);
}
