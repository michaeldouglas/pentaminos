# Pentaminós – Solver + GUI (Raylib)

Aplicação para jogar e resolver o quebra-cabeça dos pentominós utilizando grafos, DFS, BFS e AVL.

—

Como compilar (Windows)

- No PowerShell/Prompt, na pasta do projeto:
  ```bat
  .\compilar.bat
  ```
- Os binários são gerados em `build_gui`:
  - `pentaminos_play.exe` (GUI/Jogar e modos Resolver por linha de comando)
  - `pentaminos_demo.exe` (GUI com prefill padrão)

—

Como executar

- Modo interativo (menu):

  ```bat
  .\build_gui\pentaminos_play.exe 6 10
  ```

  - O programa pedirá o modo: Jogar (GUI), Resolver DFS (uma/todas), BFS, ou Comparar DFS×BFS.
  - Tamanhos suportados: qualquer `m×n` com área múltipla de 5 até 60 (ex.: 6×10, 5×12, 4×15, 3×20). O número de peças usadas é `K = área/5`.

- Modos Resolver por flags (sem menu):

  ```bat
  .\build_gui\pentaminos_play.exe 6 10 --solve=dfs         # primeira solução (DFS)
  .\build_gui\pentaminos_play.exe 6 10 --solve=dfs --all  # todas as soluções (DFS)
  .\build_gui\pentaminos_play.exe 6 10 --solve=bfs         # solução de menor profundidade (BFS)
  .\build_gui\pentaminos_play.exe 6 10 --compare           # compara DFS×BFS (estados/tempo)
  ```

- Jogar (GUI):
  - Peças à esquerda; clique para selecionar.
  - Variações: ↑/↓ (ou W/S).
  - Colocar: clique no tabuleiro.
  - Desfazer: BACKSPACE; Sair: ESC.
  - Contador mostra “Colocadas: X/Y” onde `Y = K`.
  - Opcional: prefill automático (pergunta no menu). Ou use `--prefill`.

—

Requisitos

- Windows 10+ com g++ (MinGW).
- Raylib (o script tenta automaticamente):
  - MSYS2 ucrt64 (recomendado):
    ```powershell
    pacman -S mingw-w64-ucrt-x86_64-raylib
    ```
  - vcpkg (alternativa MinGW dinâmica):
    ```powershell
    vcpkg install raylib:x64-mingw-dynamic
    ```

—

Arquitetura

- `main.cpp`: GUI (Raylib), menu e orquestração.
- `src/piece`: geração de peças e variações (`Piece.cpp/.h`).
- `src/board`: tabuleiro e operações (`Board.cpp/.h`).
- `src/state`: estado de busca (`State.cpp/.h`).
- `src/avl`: árvore AVL para visitados (`AVLTree.cpp/.h`).
- `src/graph`: solvers (`GraphSolver.cpp/.h`) com:
  - DFS (primeira e todas as soluções) e BFS (mínima profundidade), métricas de estados e tempo.

—

Notas

- A chave de estados na AVL utiliza o `grid`. Como os IDs das peças ficam no `grid`, `usedPieces` é redundante para detectar repetição de estado.
- “Todas as soluções” pode ser muito custoso; use com cautela ou em tabuleiros menores (K mais baixo).
