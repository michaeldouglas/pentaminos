
#include "Board.h"
#include <iostream>

Board::Board(int r, int c): rows(r), cols(c) {
    grid.assign(r, vector<int>(c, 0));
}
bool Board::isValid(int r, int c) const {
    return r>=0 && r<rows && c>=0 && c<cols;
}
bool Board::placePiece(const vector<pair<int,int>>& s,int r,int c,int id){
    for(auto [dr,dc]:s){
        int nr=r+dr,nc=c+dc;
        if(!isValid(nr,nc) || grid[nr][nc]!=0) return false;
    }
    for(auto [dr,dc]:s) grid[r+dr][c+dc]=id;
    return true;
}
void Board::removePiece(const vector<pair<int,int>>& s,int r,int c){
    for(auto [dr,dc]:s) grid[r+dr][c+dc]=0;
}
void Board::print() const{
    for(auto& row:grid){
        for(int v:row) std::cout<<v<<" ";
        std::cout<<"\n";
    }
}
int Board::getRows() const{return rows;}
int Board::getCols() const{return cols;}
vector<vector<int>> Board::getGrid() const{return grid;}
