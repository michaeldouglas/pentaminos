# 🎮 COMECE AQUI - Guia Rápido

## ⚡ 3 Passos para Começar

### Passo 1: Abra PowerShell

```powershell
# Na pasta do projeto
cd c:\Users\mdbaa\development\c\pentaminos
```

### Passo 2: Compile

```powershell
# Se já tem g++ instalado:
g++ -std=c++17 -I include -o pentaminos main.cpp src/*.cpp

# Se não tem, veja: INSTALAR_FERRAMENTAS.md
```

### Passo 3: Execute

```powershell
.\pentaminos.exe
```

---

## 📝 O Programa Vai Pedir:

```
Linhas: 3
Colunas: 5
1-DFS 2-BFS 3-Jogar: 3
```

**Digite:**

- Linhas: `3`
- Colunas: `5`
- Modo: `3` (para jogar interativamente)

---

## 🎮 Modo Jogar (Opção 3)

Depois de escolher modo 3, o programa mostra as peças disponíveis:

```
Pecas disponiveis:
ID 1
ID 2
ID 3
... (até 12)

Escolha ID (-1 sai):
```

**Digite:**

1. ID da peça (1-12)
2. Variação (0-7)
3. Linha (0 a 2)
4. Coluna (0 a 4)
5. Digite `-1` para sair

---

## 🚀 Versão Bonita com Interface Gráfica

Se quiser a versão com gráficos coloridos:

1. **Instale Raylib (uma vez):**

   ```powershell
   # Instalar vcpkg
   cd c:\temp
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat

   # Instalar Raylib (leva 10 minutos)
   .\vcpkg install raylib:x64-mingw-dynamic
   ```

2. **Compile a versão GUI:**

   ```powershell
   cd c:\Users\mdbaa\development\c\pentaminos
   $r = "c:\temp\vcpkg\installed\x64-mingw-dynamic"
      g++ -std=c++17 -I include -I "$r\include" -L "$r\lib" `
         -o pentaminos main.cpp src/*.cpp `
         -lraylib -lwinmm -luser32 -lgdi32
   ```

3. **Execute:**
   ```powershell
   .\pentaminos.exe
   ```

---

## 💡 Dica: Use o Menu Automático

Se acha confuso, execute:

```powershell
.\menu.ps1
```

E escolha a opção no menu! 📋

---

## ❓ Erro?

**"g++ not found"**

- Precisa instalar MinGW
- Veja: [INSTALAR_FERRAMENTAS.md](INSTALAR_FERRAMENTAS.md)

**"Caracteres estranhos"**

- Isso já foi corrigido!
- Se persistir, use versão GUI

**"GUI não funciona"**

- Instale Raylib (passo acima)

---

## 📚 Documentação Completa

Para tudo em detalhes, veja:

- [README.md](README.md) - Visão geral
- [STATUS_FINAL.md](STATUS_FINAL.md) - Tudo que foi feito
- [GUIA_INTERFACE_GRAFICA.md](GUIA_INTERFACE_GRAFICA.md) - GUI em detalhes
- [COMPILAR.md](COMPILAR.md) - Compilação
- [INSTALAR_FERRAMENTAS.md](INSTALAR_FERRAMENTAS.md) - Ferramentas

---

**Pronto? Vamos começar!** 🚀

```powershell
cd c:\Users\mdbaa\development\c\pentaminos
g++ -std=c++17 -I include -o pentaminos main.cpp src/*.cpp
.\pentaminos.exe
```
