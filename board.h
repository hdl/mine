#ifndef BOARD_H
#define BOARD_H
/* Place to put all of my definitions etc. */

typedef struct{
    int row;
    int col;
    int mine_num;
    int *mine_left;
    unsigned int seed;
    char *values;
    char *status;
    char *visit; // prepared for recsive reveal
}board_t;


char * init_board(int num_rows, int num_columns, char symbol);
void place_mine(board_t *board);
void set_hint(board_t board, int i, int j);
void place_hint(board_t board);
void print_value(char *board, int num_rows, int num_columns);
void print_status(board_t board);
#endif




