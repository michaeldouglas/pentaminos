# Pentaminós (GUI)

Interface gráfica para montar pentominós com Raylib.

—

Como compilar e executar (Windows)

- Abra um PowerShell ou Prompt de Comando na pasta do projeto e rode:
  ```bat
  .\compilar.bat
  ```
- O script compila e já inicia o jogo automaticamente (tamanho padrão do tabuleiro: 6x10).
- Para mudar o tamanho padrão, edite a última linha do arquivo `compilar.bat` e ajuste os dois números, por exemplo:
  ```bat
  build_gui\pentaminos.exe 5 12
  ```

—

Como jogar (GUI)

- Objetivo: preencher todo o tabuleiro com as 12 peças pentominó, sem sobreposição.
- Passos básicos:
  - Painel esquerdo: clique em uma peça para selecioná-la (peças usadas ficam marcadas).
  - Variações: use as teclas ↑/↓ (ou W/S) para alternar entre rotações/reflexões da peça selecionada.
  - Colocar peça: clique no tabuleiro (área da direita) para posicionar a peça na célula desejada.
  - Desfazer: pressione BACKSPACE para remover a última peça colocada.
  - Sair: pressione ESC para fechar a janela.
- Dicas:
  - Você verá um preview/indicador e a contagem “Colocadas: X/12”.
  - Se não couber, troque a variação (↑/↓) e tente novamente.
  - Ao completar 12/12, aparece a mensagem de vitória.

—

Requisitos

- Windows 10+ com g++ (MinGW) instalado.
- Raylib instalado (o script tenta automaticamente com um destes ambientes):
  - MSYS2 ucrt64 (recomendado):
    ```powershell
    pacman -S mingw-w64-ucrt-x86_64-raylib
    ```
  - vcpkg (alternativa MinGW dinâmica):
    ```powershell
    vcpkg install raylib:x64-mingw-dynamic
    ```

—

Estrutura do projeto

- `main.cpp` Interface gráfica (Raylib)
- `include/` Headers (Board.h, Piece.h, State.h, AVLTree.h, GraphSolver.h)
- `src/` Implementações (.cpp)
- `compilar.bat` Compila e executa (não interativo)

—

Créditos e status

- Versão: 2.0 (GUI)
- Status: Pronto para usar
