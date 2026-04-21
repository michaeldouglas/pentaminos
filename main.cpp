#include "raylib.h"
#include "Board.h"
#include "Piece.h"
#include "GraphSolver.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include "AVLTree.h"
#include "State.h"

using namespace std;

const int CELL_SIZE = 50;
const int MARGIN = 20;
const int UI_WIDTH = 300;

// Define custom color CORAL (not provided by raylib by default)
static const Color CORAL = {255, 127, 80, 255};

Color pieceColors[] = {
    RED, BLUE, GREEN, YELLOW, ORANGE, PURPLE,
    BROWN, PINK, SKYBLUE, LIME, MAGENTA, CORAL};

struct PlacedPiece
{
    Piece piece;
    vector<pair<int, int>> shape;
    int row, col;
    PlacedPiece(const Piece &p, const vector<pair<int, int>> &s, int r, int c)
        : piece(p), shape(s), row(r), col(c) {}
};

// Função auxiliar para encontrar índice de peça
int getPieceIndex(const vector<Piece> &pieces, int id)
{
    for (int i = 0; i < (int)pieces.size(); i++)
    {
        if (pieces[i].id == id)
            return i;
    }
    return -1;
}

// Forward declaration do solver de preenchimento automatico
static bool autoDfs(Board &board, vector<Piece> &pieces, int idx, AVLTree &vis);

int main(int argc, char **argv)
{
    int rows = 0, cols = 0;
    if (argc >= 3)
    {
        rows = std::atoi(argv[1]);
        cols = std::atoi(argv[2]);
    }
    else
    {
        cout << "Linhas: ";
        cin >> rows;
        cout << "Colunas: ";
        cin >> cols;
    }

    if (rows <= 0 || cols <= 0)
    {
        cout << "Dimensoes invalidas: use valores positivos.\n";
        return 1;
    }
    // Validacao: area multipla de 5 e <= 60 (12 pentaminos). K = area/5
    int area = rows * cols;
    if (area % 5 != 0 || area > 60)
    {
        cout << "Area invalida. Use multiplos de 5 ate 60 (ex: 6x10, 5x12, 4x15, 3x20).\n";
        return 1;
    }
    int K = area / 5;

    cout << "Gerando o jogo..." << endl;
    // Se resolver via menu (DFS uma) e quisermos abrir a GUI preenchida
    vector<vector<int>> solvedGrid;
    bool solvedForGUI = false;

    // Modo Resolver via argumentos: --solve=dfs|bfs [--all]
    if (argc >= 4)
    {
        std::string mode = argv[3];
        if (mode == "--solve=dfs" || mode == "--solve=bfs")
        {
            GraphSolver solver(Board(rows, cols), (size_t)K);
            int states = 0;
            long long ms = 0;
            State sol;
            if (mode == "--solve=dfs")
            {
                bool all = (argc >= 5 && std::string(argv[4]) == "--all");
                if (all)
                {
                    size_t count = solver.solveDFSAll(states, ms, 0, false);
                    std::cout << "DFS(todas): solucoes=" << count << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
                }
                else
                {
                    bool ok = solver.solveDFSOne(states, ms, sol);
                    std::cout << "DFS(uma): encontrado=" << (ok ? 1 : 0) << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
                }
            }
            else if (mode == "--solve=bfs")
            {
                bool ok = solver.solveBFS(states, ms, sol);
                std::cout << "BFS: encontrado=" << (ok ? 1 : 0) << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
            }
            return 0;
        }
        else if (mode == "--compare")
        {
            GraphSolver s1(Board(rows, cols), (size_t)K);
            GraphSolver s2(Board(rows, cols), (size_t)K);
            int st;
            long long t;
            State so;
            bool ok1 = s1.solveDFSOne(st = 0, t = 0, so); // we set via call, initializers ignored
            int states1 = st;
            long long ms1 = t;
            bool ok2 = s2.solveBFS(st = 0, t = 0, so);
            int states2 = st;
            long long ms2 = t;
            std::cout << "COMPARE (K=" << K << ")\n";
            std::cout << "DFS: ok=" << ok1 << ", estados=" << states1 << ", ms=" << ms1 << "\n";
            std::cout << "BFS: ok=" << ok2 << ", estados=" << states2 << ", ms=" << ms2 << "\n";
            return 0;
        }
    }

    // Menu interativo (quando nenhum modo foi passado por argumento)
    cout << "\n=== MODO ===\n";
    cout << "1) Jogar (GUI)\n";
    cout << "2) Resolver DFS (uma)\n";
    cout << "3) Resolver DFS (todas)\n";
    cout << "4) Resolver BFS (minima profundidade)\n";
    cout << "5) Comparar DFS x BFS\n";
    cout << "Opcao: ";
    int escolha = 1;
    cin >> escolha;
    if (escolha >= 2 && escolha <= 5)
    {
        GraphSolver solver(Board(rows, cols), (size_t)K);
        int states = 0;
        long long ms = 0;
        State sol;
        if (escolha == 2)
        {
            bool ok = solver.solveDFSOne(states, ms, sol);
            std::cout << "DFS(uma): encontrado=" << (ok ? 1 : 0) << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
            if (ok)
            {
                solvedGrid = sol.grid;
                solvedForGUI = true;
                // segue para abrir a GUI com o tabuleiro preenchido
            }
            else
            {
                return 0; // sem solucao, encerra
            }
        }
        else if (escolha == 3)
        {
            size_t count = solver.solveDFSAll(states, ms, 0, false);
            std::cout << "DFS(todas): solucoes=" << count << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
            return 0;
        }
        else if (escolha == 4)
        {
            bool ok = solver.solveBFS(states, ms, sol);
            std::cout << "BFS: encontrado=" << (ok ? 1 : 0) << ", estados=" << states << ", tempo(ms)=" << ms << "\n";
            if (ok)
            {
                solvedGrid = sol.grid;
                solvedForGUI = true;
            }
            else
            {
                return 0;
            }
        }
        else if (escolha == 5)
        {
            GraphSolver s1(Board(rows, cols), (size_t)K);
            GraphSolver s2(Board(rows, cols), (size_t)K);
            int st;
            long long t;
            State so;
            bool ok1 = s1.solveDFSOne(st = 0, t = 0, so);
            int states1 = st;
            long long ms1 = t;
            bool ok2 = s2.solveBFS(st = 0, t = 0, so);
            int states2 = st;
            long long ms2 = t;
            std::cout << "COMPARE (K=" << K << ")\n";
            std::cout << "DFS: ok=" << ok1 << ", estados=" << states1 << ", ms=" << ms1 << "\n";
            std::cout << "BFS: ok=" << ok2 << ", estados=" << states2 << ", ms=" << ms2 << "\n";
            return 0;
        }
    }

    // Opcional: perguntar prefill para o modo Jogar
    bool askPrefill = false;
    if (!solvedForGUI)
    {
        cout << "Ativar prefill automatico? (s/n): ";
        char ans = 'n';
        cin >> ans;
        askPrefill = (ans == 's' || ans == 'S');
    }

    Board board(rows, cols);
    vector<Piece> pieces = generatePentominoes();
    if ((int)pieces.size() > K)
        pieces.erase(pieces.begin() + K, pieces.end());
    vector<bool> usedPieces(pieces.size(), false);
    vector<PlacedPiece> placed;

    // Opcional: preenche automaticamente se for um tabuleiro 6x10 (12 pentominos)
    // Passe "--prefill" como terceiro argumento para tentar sempre, independente do tamanho
    // Prefill control: runtime flag or compile-time default
    bool doPrefill = false;
#ifdef PREFILL_DEFAULT
    doPrefill = true;
#endif
    if (argc >= 4 && std::string(argv[3]) == "--prefill")
        doPrefill = true;
    if (argc < 4) // se menu interativo foi usado
        doPrefill = askPrefill;
    if (solvedForGUI)
        doPrefill = false;

    if (solvedForGUI)
    {
        // Inicializa o tabuleiro com a solucao encontrada e marca pecas usadas
        board = Board(solvedGrid);
        auto g = board.getGrid();
        for (int r = 0; r < (int)g.size(); ++r)
            for (int c = 0; c < (int)g[r].size(); ++c)
                if (g[r][c] > 0 && g[r][c] - 1 < (int)usedPieces.size())
                    usedPieces[g[r][c] - 1] = true;
    }
    else if (doPrefill)
    {
        cout << "Preenchendo automaticamente, aguarde..." << endl;
        AVLTree vis;
        auto piecesCopy = pieces; // manter ordem original
        bool ok = autoDfs(board, piecesCopy, 0, vis);
        if (ok)
        {
            // Marca todas as pecas usadas com base no grid final
            auto g = board.getGrid();
            for (int r = 0; r < (int)g.size(); ++r)
                for (int c = 0; c < (int)g[r].size(); ++c)
                    if (g[r][c] > 0 && g[r][c] - 1 < (int)usedPieces.size())
                        usedPieces[g[r][c] - 1] = true;
            cout << "Pronto!" << endl;
        }
    }

    int screenWidth = UI_WIDTH + MARGIN * 2 + cols * CELL_SIZE;
    int screenHeight = MARGIN * 2 + rows * CELL_SIZE;

    // Permitir redimensionar/maximizar a janela
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Pentaminos");
    SetWindowMinSize(screenWidth, screenHeight);
    SetTargetFPS(60);

    int selectedPiece = -1;
    int selectedVariation = 0;

    while (!WindowShouldClose())
    {
        // INPUT
        // F11: alterna modo fullscreen (opcional)
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        // Atualiza dimensoes se a janela foi redimensionada
        if (IsWindowResized())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }
        if (IsKeyPressed(KEY_LEFT))
            selectedPiece = -1;
        if (IsKeyPressed(KEY_RIGHT))
            selectedPiece = -1;

        // BACKSPACE para remover ultima peca
        if (IsKeyPressed(KEY_BACKSPACE) && !placed.empty())
        {
            PlacedPiece last = placed.back();
            board.removePiece(last.shape, last.row, last.col);
            placed.pop_back();
            usedPieces[getPieceIndex(pieces, last.piece.id)] = false;
            selectedPiece = -1;
        }

        // MOUSE - selecionar peca na lista
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();

            // Clique na lista de peças (lado esquerdo)
            if (mouseX < UI_WIDTH)
            {
                int pieceIdx = (mouseY - MARGIN) / (CELL_SIZE + 5);
                if (pieceIdx >= 0 && pieceIdx < (int)pieces.size())
                {
                    if (!usedPieces[pieceIdx])
                    {
                        selectedPiece = pieceIdx;
                        selectedVariation = 0;
                    }
                }
            }
            // Clique no tabuleiro
            else if (selectedPiece >= 0)
            {
                int boardX = (mouseX - UI_WIDTH - MARGIN) / CELL_SIZE;
                int boardY = (mouseY - MARGIN) / CELL_SIZE;

                Piece &piece = pieces[selectedPiece];
                if (boardX >= 0 && boardX < cols && boardY >= 0 && boardY < rows)
                {
                    if (selectedVariation < (int)piece.variations.size())
                    {
                        if (board.placePiece(piece.variations[selectedVariation], boardY, boardX, piece.id))
                        {
                            placed.emplace_back(piece, piece.variations[selectedVariation], boardY, boardX);
                            usedPieces[selectedPiece] = true;
                            selectedPiece = -1;
                        }
                    }
                }
            }
        }

        // SETAS para trocar variacao
        if (selectedPiece >= 0)
        {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                Piece &p = pieces[selectedPiece];
                selectedVariation = (selectedVariation - 1 + p.variations.size()) % p.variations.size();
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                Piece &p = pieces[selectedPiece];
                selectedVariation = (selectedVariation + 1) % p.variations.size();
            }
        }

        // DRAW
        BeginDrawing();
        ClearBackground(WHITE);

        // PAINEL ESQUERDO - Lista de peças
        DrawRectangle(0, 0, UI_WIDTH, screenHeight, LIGHTGRAY);
        DrawText("Pecas", MARGIN, MARGIN, 20, BLACK);

        for (int i = 0; i < (int)pieces.size(); i++)
        {
            int y = MARGIN + 30 + i * (CELL_SIZE + 5);
            Color bgColor = usedPieces[i] ? DARKGRAY : (i == selectedPiece ? YELLOW : WHITE);
            DrawRectangle(MARGIN, y, UI_WIDTH - 2 * MARGIN, CELL_SIZE, bgColor);
            DrawRectangleLines(MARGIN, y, UI_WIDTH - 2 * MARGIN, CELL_SIZE, BLACK);

            string text = "Peca " + to_string(pieces[i].id);
            if (usedPieces[i])
                text += " [USADO]";
            DrawText(text.c_str(), MARGIN + 5, y + 5, 14, BLACK);
        }

        // PAINEL CENTRAL - Tabuleiro
        int boardStartX = UI_WIDTH + MARGIN;
        int boardStartY = MARGIN;

        DrawText("Tabuleiro", boardStartX, boardStartY - 25, 20, BLACK);

        // Desenhar grid
        for (int i = 0; i <= rows; i++)
        {
            DrawLine(boardStartX, boardStartY + i * CELL_SIZE,
                     boardStartX + cols * CELL_SIZE, boardStartY + i * CELL_SIZE, BLACK);
        }
        for (int j = 0; j <= cols; j++)
        {
            DrawLine(boardStartX + j * CELL_SIZE, boardStartY,
                     boardStartX + j * CELL_SIZE, boardStartY + rows * CELL_SIZE, BLACK);
        }

        // Desenhar peças colocadas
        auto grid = board.getGrid();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (grid[i][j] != 0)
                {
                    int x = boardStartX + j * CELL_SIZE;
                    int y = boardStartY + i * CELL_SIZE;
                    DrawRectangle(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4,
                                  pieceColors[grid[i][j] - 1]);
                    DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, BLACK);
                }
            }
        }

        // Desenhar preview da peça selecionada
        if (selectedPiece >= 0 && selectedVariation < (int)pieces[selectedPiece].variations.size())
        {
            Piece &p = pieces[selectedPiece];
            auto &var = p.variations[selectedVariation];

            // Preview no canto
            int previewX = boardStartX + 10;
            int previewY = boardStartY + rows * CELL_SIZE + 30;

            DrawText("Preview", previewX, previewY - 25, 14, BLACK);
            for (auto [r, c] : var)
            {
                DrawRectangle(previewX + c * 25, previewY + r * 25, 22, 22, pieceColors[p.id - 1]);
                DrawRectangleLines(previewX + c * 25, previewY + r * 25, 24, 24, BLACK);
            }

            DrawText(("Var " + to_string(selectedVariation)).c_str(),
                     previewX + 100, previewY, 12, BLUE);
            DrawText("Setas: trocar | Clique: colocar", previewX, previewY + 100, 11, DARKGRAY);
        }

        // INFO
        DrawText("PENTAMINOS - Interface Grafica",
                 boardStartX, boardStartY + rows * CELL_SIZE + 30, 16, BLACK);

        int usedCount = 0;
        for (bool u : usedPieces)
            if (u)
                usedCount++;
        string info = "Colocadas: " + to_string(usedCount) + "/" + to_string((int)pieces.size());
        DrawText(info.c_str(), boardStartX, boardStartY + rows * CELL_SIZE + 55, 12, BLACK);

        // Mensagem de vitória
        if (usedCount == (int)pieces.size())
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});
            DrawText("PARABENS! VENCEU!",
                     screenWidth / 2 - 150, screenHeight / 2 - 30, 40, GREEN);
        }

        // Instruções
        DrawText("BACKSPACE: desfazer | ESC: sair", MARGIN, screenHeight - 30, 12, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Resolver automaticamente (DFS com poda via AVLTree)
static bool autoDfs(Board &board, vector<Piece> &pieces, int idx, AVLTree &vis)
{
    State st(board.getGrid(), {});
    if (vis.contains(st))
        return false;
    vis.insert(st);
    if (idx == (int)pieces.size())
        return true;
    Piece &p = pieces[idx];
    for (auto &var : p.variations)
    {
        for (int i = 0; i < board.getRows(); i++)
        {
            for (int j = 0; j < board.getCols(); j++)
            {
                if (board.placePiece(var, i, j, p.id))
                {
                    if (autoDfs(board, pieces, idx + 1, vis))
                        return true;
                    board.removePiece(var, i, j);
                }
            }
        }
    }
    return false;
}