# sCdoku - Sudoku Competitivo com Interface Gráfica

Uma implementação do jogo Sudoku com interface gráfica usando a biblioteca Allegro5, onde dois jogadores competem para resolver o tabuleiro.

## Instalação do Allegro5

### No Ubuntu/Debian:
```bash
# Atualize os pacotes
sudo apt-get update

# Instale as dependências básicas
sudo apt-get install build-essential

# Instale o Allegro5 e suas bibliotecas
sudo apt-get install liballegro5-dev liballegro-acodec5-dev liballegro-audio5-dev \
    liballegro-color5-dev liballegro-dialog5-dev liballegro-image5-dev \
    liballegro-ttf5-dev
```

### No Fedora:
```bash
# Instale o Allegro5 e suas bibliotecas
sudo dnf install allegro5-devel allegro5-image-devel allegro5-font-devel
```

### No Windows:
1. Baixe o instalador do Allegro5 do site oficial: https://liballeg.org/download.html
2. Execute o instalador e siga as instruções na tela
3. Adicione o caminho do Allegro5 ao PATH do sistema:
   - Clique com o botão direito no Menu Iniciar
   - Selecione "Sistema"
   - Clique em "Configurações do Sistema"
   - Clique em "Variáveis de Ambiente"
   - Em "Variáveis do Sistema", selecione "Path"
   - Clique em "Editar"
   - Clique em "Novo"
   - Adicione o caminho onde o Allegro5 foi instalado (exemplo: C:\allegro5\bin)

## Como Compilar e Executar

```bash
# Clone o repositório
$ git clone https://github.com/ArturCSegat/sCdoku
$ cd sCdoku

# Compile o projeto
$ make

# Execute o jogo
$ ./bin/Debug/sudoku.out
```

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
