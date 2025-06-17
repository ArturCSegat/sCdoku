#include "game.c"
#include "utils.c"


int main(void) {
    Game g= new_game();
    printf("move format: 1 - 4:5=8\nplayer one plays 8 on row 4 col 5\n");
    while(1) {
        print_game(&g);
        printf("enter your move:\n");
        int row, col, p, val;
        scanf("%d - %d:%d=%d", &p, &row, &col, &val);
        play(move(val+'0', row, col, p==1),  &g);
        clear_screen();
    }
}
