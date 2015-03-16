#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"



char * init_board(int num_rows, int num_columns, char symbol){
    int i=0, j=0;
    char *board = (char *)malloc(sizeof(char) * num_columns * num_rows);
    for(i=0; i<num_rows; i++)
        for(j=0; j<num_columns; j++)
            board[i * num_columns + j]=symbol;
    return board;
}



void place_mine(board_t *board){
    int rand_row=0;
    int rand_col=0;
    int counter=0;
    //printf("seed is %d", board->seed);
    srand(board->seed);
    while(1){
        rand_row = rand() % board->row;
        rand_col = rand() % board->col;

        if(board->values[rand_row * board->col + rand_col] != '*'){
            printf("Placing mine at %d, %d\n", rand_row, rand_col);
            board->values[rand_row * board->col + rand_col]='*';
            counter++;
        }else{
            continue;
        }
        if(counter == board->mine_num)
            return;

    }

}

void set_hint(board_t board, int i, int j)
{
    if( i<0 || i>=board.row || j<0 || j>=board.col)
        return;
    else{
        if(board.values[i*board.col + j]!='*')
            board.values[i*board.col + j]++;

    }
    return;

}
void place_hint(board_t board){
    int i, j;
    for(i=0; i<board.row; i++)
        for(j=0; j<board.col; j++){
            if(board.values[i*board.col + j] == '*'){
                set_hint(board, i-1, j-1);
                set_hint(board, i-1, j);
                set_hint(board, i-1, j+1);
                set_hint(board, i, j-1);
                set_hint(board, i, j+1);
                set_hint(board, i+1, j-1);
                set_hint(board, i+1, j);
                set_hint(board, i+1, j+1);
            }

        }
    for(i=0; i<board.row; i++)
        for(j=0; j<board.col; j++)
            if(board.values[i*board.col + j] != '*')
                board.values[i*board.col + j] +='0';
}











