#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <utility>
using namespace std;

class Board
{
private:
    int rows, cols;
    vector<vector<int>> grid;

public:
    Board(int r, int c);
    bool placePiece(const vector<pair<int, int>> &shape, int r, int c, int id);
    void removePiece(const vector<pair<int, int>> &shape, int r, int c);
    bool isValid(int r, int c) const;
    void print() const;
    int getRows() const;
    int getCols() const;
    vector<vector<int>> getGrid() const;
};
#endif
