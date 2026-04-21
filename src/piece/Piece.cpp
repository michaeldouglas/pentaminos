
#include "Piece.h"
#include <set>
#include <algorithm>

static vector<pair<int,int>> normalize(vector<pair<int,int>> s){
    int mr=1e9, mc=1e9;
    for(auto [r,c]:s){ mr=min(mr,r); mc=min(mc,c); }
    for(auto &p:s){ p.first-=mr; p.second-=mc; }
    sort(s.begin(), s.end());
    return s;
}
static vector<pair<int,int>> rotate90(vector<pair<int,int>> s){
    vector<pair<int,int>> r;
    for(auto [x,y]:s) r.push_back({y, -x});
    return normalize(r);
}
static vector<pair<int,int>> reflectH(vector<pair<int,int>> s){
    vector<pair<int,int>> r;
    for(auto [x,y]:s) r.push_back({x, -y});
    return normalize(r);
}
static vector<vector<pair<int,int>>> genVars(vector<pair<int,int>> base){
    set<vector<pair<int,int>>> u;
    auto cur = normalize(base);
    for(int i=0;i<4;i++){
        u.insert(cur);
        u.insert(reflectH(cur));
        cur = rotate90(cur);
    }
    return vector<vector<pair<int,int>>>(u.begin(), u.end());
}

Piece::Piece(int id, vector<vector<pair<int,int>>> vars): id(id), variations(vars) {}

vector<Piece> generatePentominoes(){
    vector<Piece> p;
    p.push_back(Piece(1, genVars({{0,0},{1,0},{2,0},{3,0},{4,0}}))); // I
    p.push_back(Piece(2, genVars({{0,0},{1,0},{2,0},{3,0},{3,1}}))); // L
    p.push_back(Piece(3, genVars({{0,0},{0,1},{1,0},{1,1},{2,0}}))); // P
    p.push_back(Piece(4, genVars({{0,0},{0,1},{0,2},{1,1},{2,1}}))); // T
    p.push_back(Piece(5, genVars({{0,0},{0,2},{1,0},{1,1},{1,2}}))); // U
    p.push_back(Piece(6, genVars({{0,0},{1,0},{2,0},{2,1},{2,2}}))); // V
    p.push_back(Piece(7, genVars({{0,0},{1,0},{1,1},{2,1},{2,2}}))); // W
    p.push_back(Piece(8, genVars({{1,0},{0,1},{1,1},{2,1},{1,2}}))); // X
    p.push_back(Piece(9, genVars({{0,0},{1,0},{2,0},{3,0},{2,1}}))); // Y
    p.push_back(Piece(10, genVars({{0,0},{0,1},{1,1},{2,1},{2,2}}))); // Z
    p.push_back(Piece(11, genVars({{0,1},{1,0},{1,1},{1,2},{2,2}}))); // F
    p.push_back(Piece(12, genVars({{0,0},{1,0},{1,1},{2,1},{3,1}}))); // N
    return p;
}
