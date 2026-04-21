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
    bool isComplete(const std::vector<std::vector<int>> &g) const;

public:
    GraphSolver(Board b, size_t pieceCount = 12);
    // DFS: primeira solucao
    bool solveDFSOne(int &states, long long &ms, State &solution);
    // DFS: todas as solucoes (maxSolutions=0 para ilimitado). Retorna contagem.
    size_t solveDFSAll(int &states, long long &ms, size_t maxSolutions = 0, bool printSolutions = false);
    // BFS: primeira solucao minima em profundidade
    bool solveBFS(int &states, long long &ms, State &solution);
    void playMode();
    void listPieces();
    void showPieceVariations(const Piece &p);
};
#endif
