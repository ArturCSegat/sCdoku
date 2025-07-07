# sCdoku - Sudoku Competitivo com Interface Gráfica

Uma implementação do jogo Sudoku com interface gráfica usando a biblioteca Allegro5, onde dois jogadores competem para resolver o tabuleiro.

## Instalação do Allegro5

### No linux:
Veja a sessão sobre Docker (Está em ingles pq eu pedi pro gpt)

### No Windows:
1. Baixe o [allegro e o compilador](http://www.dropbox.com/s/jaswa5mw3nb3ogn/Allegro.zip?dl=1)
2. Descompate e ponha em C:\Allegro
3. clone o repositório
```bash
$ git clone https://github.com/ArturCSegat/sCdoku
```
4. abra o arquivo .cbp no codeblocks
5. Clique em projeto e inclua os arquivos, todos menos os com o sufixo _unix
6. Vá em configurações e em 'toolchain executables'
7. Selecione o compilador C:\Allegro\AllegroMingW
8. Configure o compilador de C para g++.exe
9. Tente rodar o programa
10. Boa sorte 

## Requisitos
- GCC ou outro compilador C (MinGW no Windows)
- Allegro5 (versão 5.2 ou superior)
- Bibliotecas adicionais do Allegro5:
  - allegro_font (para renderização de texto)
  - allegro_image (para carregar e manipular imagens)
  - allegro_ttf (para fontes TrueType)

## Funcionalidades
- Interface gráfica moderna com Allegro5
- Jogo de Sudoku competitivo para 2 jogadores
- Sistema de pontos e vidas:
  - Cada jogador começa com 5 vidas
  - Pontos são ganhos por jogadas válidas
  - Jogadas inválidas resultam em perda de vida
- Validação de jogadas em tempo real
- Tabuleiro 9x9 com regras padrão de Sudoku
- Controle de tempo e turnos
- Sistema de placar visível durante o jogo


## Rodar no Docker (Exclusivo Linux)

### 🐳 Requirements

- Docker installed
- Linux system (for `--network host` and X11 GUI support)
- X11 server (already available on most Linux desktops)

### 1. Clone the project

```bash
git clone https://github.com/youruser/yourproject.git
cd yourproject
```

### Build the Docker image

```bash
docker build -t scdoku .
```

### Allow X11 access (on the host)

```
xhost +local:root
```

### Run the container

```
docker run \
  --network host \
  --env DISPLAY=$DISPLAY \
  --volume /tmp/.X11-unix:/tmp/.X11-unix \
  scdoku
```

