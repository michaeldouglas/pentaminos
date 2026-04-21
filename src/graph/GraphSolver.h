#ifndef GRAPH_SOLVER_H
#define GRAPH_SOLVER_H
#include "Board.h"
#include "AVLTree.h"
#include "Piece.h"
#include <vector>

class GraphSolver
{
private:
    Board board;
    AVLTree visited;
    std::vector<Piece> pieces;

public:
    GraphSolver(Board b);
    void solveDFS();
    void solveBFS();
    void playMode();
    void listPieces();
    void showPieceVariations(const Piece &p);
};
#endif
