
#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include <utility>
using namespace std;

class Piece {
public:
    int id;
    vector<vector<pair<int,int>>> variations;
    Piece(int id, vector<vector<pair<int,int>>> vars);
};

vector<Piece> generatePentominoes();

#endif
