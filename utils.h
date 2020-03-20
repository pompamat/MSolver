#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::max;

void print_str_matrix(const vector<vector<string>>& V){
    vector<int> col_begin;
    col_begin.push_back(0);
    for(int i = 0; i < V[0].size(); ++i){
        int dl = 0;
        for(int j = 0; j < V.size(); ++j) dl = max(dl, (int) V[j][i].size());
        col_begin.push_back(1 + dl + col_begin.back());
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
