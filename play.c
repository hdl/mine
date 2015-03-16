
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "play.h"
void make_action(board_t board, int action_row, int action_col, char action){

    board.status[action_row * board.col + action_col] = action;

    if(action == '!'){
        (*board.mine_left)--;
    }
    if(action == 'r' && board.values[action_row * board.col + action_col]=='*'){
        print_value(board.values, board.row, board.col);
        printf("You Lost :(\n");
        exit(0);
    }

    if(action == 'r' && board.values[action_row * board.col + action_col]=='0'){
        //printf("here?\n");
        recsive_reveal(board, action_row, action_col);
    }
    win(board, action_row, action_col);

}


int win(board_t board, int row, int col){
    int i,j;
    for(i=0; i<board.row; i++)
        for(j=0; j<board.col; j++){
            if(board.values[i*board.col+j] != '*' && board.status[i*board.col+j]!='r'){
                //printf("return because %d %d", i, j);
                return 0;
            }

            if(board.values[i*board.col+j] == '*' && board.status[i*board.col+j]!='!'){
                //printf("return because %d %d", i, j);
                return 0;
            }

        }
    print_value(board.values, board.row, board.col);
    printf("You Won!!\n");
    exit(0);
    return 1;
}


void recsive_reveal(board_t board, int action_row, int action_col){


    //printf("current: %d, %d\n", action_row, action_col);
    if(action_col<0 || action_row<0 || action_row>=board.row || action_col >= board.col ){ //terminated
        //printf("return because terminated\n");
        return;
    }
    if( board.status[action_row*board.col +action_col]=='?' || board.status[action_row*board.col +action_col]=='!' ){

        board.visit[action_row * board.col + action_col]=1;
        return;
    }

    if(board.values[action_row * board.col + action_col]!='0'){  // base
        //printf("return because base");
        board.status[action_row * board.col + action_col]='r';
        return;
    }

    if(board.visit[action_row * board.col + action_col]==1){  // pruning
        //printf("return because visted\n");
        return;
    }
    if(board.values[action_row * board.col + action_col]=='0'){
        board.visit[action_row * board.col + action_col]=1;
        board.status[action_row * board.col + action_col]='r';
    }

    if(board.values[action_row*board.col + action_col] == '0'){
        recsive_reveal(board, action_row-1, action_col-1);
        recsive_reveal(board, action_row-1, action_col);
        recsive_reveal(board, action_row-1, action_col+1);
        recsive_reveal(board, action_row, action_col-1);
        recsive_reveal(board, action_row, action_col+1);
        recsive_reveal(board, action_row+1, action_col-1);
        recsive_reveal(board, action_row+1, action_col);
        recsive_reveal(board, action_row+1, action_col+1);
    }
    return ;
}
