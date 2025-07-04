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

### No Linux:
```bash
# Clone o repositório
$ git clone https://github.com/ArturCSegat/sCdoku
$ cd sCdoku

# Compile o projeto
$ gcc main.c -o sudoku.out -lallegro -lallegro_font -lallegro_image

# Execute o jogo
$ ./sudoku.out
```

### No Windows:
```bash
# Clone o repositório
> git clone https://github.com/ArturCSegat/sCdoku
> cd sCdoku

# Compile o projeto
> gcc main.c -o sudoku.exe -lallegro -lallegro_font -lallegro_image

# Execute o jogo
> sudoku.exe
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

## Como Jogar
1. O jogo começa com um tabuleiro parcialmente preenchido
2. Os jogadores se alternam em turnos
3. Cada jogador deve preencher uma célula vazia com um número de 1 a 9
4. As regras básicas do Sudoku se aplicam:
   - Números não podem se repetir na mesma linha
   - Números não podem se repetir na mesma coluna
   - Números não podem se repetir no mesmo bloco 3x3
5. O jogo termina quando:
   - O tabuleiro é completamente preenchido
   - Um jogador perde todas as vidas
   - Um jogador consegue mais pontos que o outro e completa o tabuleiro

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

```
sCdoku/
├── README.md               # Documentação do projeto
├── main.c                  # Ponto de entrada do programa e configuração da interface gráfica
├── src/
│   ├── control/
│   │   └── handle.c       # Gerenciamento de eventos e interações do usuário
│   ├── game/
|   |   └── game.c         # Lógica do Jogo
|   |   └── sudoku.c       # Estruturas de dados
|   ├── draw/
|   |   └── draw_rooms.c   # Foco em desenhar as salas
|   ├── states/
|   |   └── room.c         # Estrutura de dado para controle da sala
├── assets/                # Recursos gráficos e fontes
└── include/               # Arquivos de cabeçalho
```

### Descrição dos Componentes

- `main.c`: Contém a função principal e a configuração inicial da interface gráfica usando Allegro5
- `src/control/handle.c`: Gerencia eventos de entrada do usuário, como cliques e teclas
- `src/game.c`: Implementa a lógica do jogo, incluindo turnos e pontuação
- `src/sudoku.c`: Contém o algoritmo de geração e validação do tabuleiro de Sudoku
- `src/utils.c`: Funções auxiliares para operações comuns, como validação de números e geração de números aleatórios
- `assets/`: Diretório para armazenar recursos gráficos, como imagens e fontes
- `include/`: Diretório para arquivos de cabeçalho (.h) que definem as interfaces das funções e estruturas de dados

## Dicas para Desenvolvimento
- Use o GCC com flags de otimização para melhor performance:
  ```bash
  gcc -O2 main.c -o sudoku.out -lallegro -lallegro_font -lallegro_image
  ```
- Para depuração, use a flag -g:
  ```bash
  gcc -g main.c -o sudoku.out -lallegro -lallegro_font -lallegro_image
  ```
- Certifique-se que todas as bibliotecas do Allegro5 estão corretamente instaladas e no PATH
