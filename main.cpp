#include "raylib.h"
#include "Board.h"
#include "Piece.h"
#include <vector>
#include <iostream>
#include <cstdlib>

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

    if ((rows * cols) % 5 != 0)
    {
        cout << "Area deve ser multipla de 5\n";
        return 1;
    }

    Board board(rows, cols);
    vector<Piece> pieces = generatePentominoes();
    vector<bool> usedPieces(pieces.size(), false);
    vector<PlacedPiece> placed;

    int screenWidth = UI_WIDTH + MARGIN * 2 + cols * CELL_SIZE;
    int screenHeight = MARGIN * 2 + rows * CELL_SIZE;

    InitWindow(screenWidth, screenHeight, "Pentaminos");
    SetTargetFPS(60);

    int selectedPiece = -1;
    int selectedVariation = 0;

    while (!WindowShouldClose())
    {
        // INPUT
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

        string info = "Colocadas: " + to_string(placed.size()) + "/12";
        DrawText(info.c_str(), boardStartX, boardStartY + rows * CELL_SIZE + 55, 12, BLACK);

        // Mensagem de vitória
        if (placed.size() == 12)
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