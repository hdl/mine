#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "play.h"

int main(int argc, char *argv[])
{
    board_t board;
    int action_row=0;
    int action_col=0;
    int action;
    int left;
    if(argc <4){
        printf("Not enough arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        return 0;
    }
    if(argc >6){
        printf("Too many arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        return 0;
    }
    board.row = atoi(argv[1]);
    board.col = atoi(argv[2]);
    board.mine_num = atoi(argv[3]);
    left = atoi(argv[3]);
    board.mine_left = &left;
    if(argc == 5)
        board.seed = atoi(argv[4]);
    else
        board.seed = time(NULL); // need change to time


    //printf("row %d, col %d, mine_num %d\n", board.row, board.col, board.mine_num);

    board.values = init_board(board.row, board.col, 0);
    board.status = init_board(board.row, board.col, '#');
    board.visit = init_board(board.row, board.col, 0);

    place_mine(&board);
    place_hint(board);
    print_status(board);

    //print_value(board.values, board.row, board.col);

    while(1){
        printf("Enter row a row between 0-%d and a column between 0-%d: ", board.row-1, board.col-1);
        scanf("%d %d", &action_row, &action_col);
        if(action_row<0 || action_row>board.row-1 || action_col<0 || action_col> board.col-1)
            continue;
        if(board.status[action_row * board.col + action_col]=='r'){
            printf("This tile is already revealed.\n");
            continue;
        }
        if(board.status[action_row * board.col + action_col]!='?' && board.status[action_row * board.col + action_col]!='!'){
            printf("Enter Action\n0. Reveal\n1. Question\n2. Mark\n3. Cancel\nAction: ");
            scanf("%d", &action);

            if(action == 0){
                if(board.status[action_row * board.col + action_col] == 'r'){
                    printf("This tile is already revealed.\n");
                    continue;
                }else
                    make_action(board, action_row, action_col, 'r');
            }
            else if(action == 1)
                make_action(board, action_row, action_col, '?');
            else if(action == 2)
                make_action(board, action_row, action_col, '!');
            else
                continue;
            print_status(board);
        }else if(board.status[action_row * board.col + action_col]=='?'){
            printf("Enter Action\n0. UnQuestion\n1. Cancel\nAction: ");
            scanf("%d", &action);
            if(action == 0)
                board.status[action_row * board.col + action_col] = '#';
            else
                continue;
            print_status(board);
        }else if(board.status[action_row * board.col + action_col]=='!'){
            printf("Enter Action\n0. UnMark\n1. Cancel\nAction: ");
            scanf("%d", &action);
            if(action == 0){
                board.status[action_row * board.col + action_col] = '#';
                left++;
            }
            else
                continue;
            print_status(board);
        }


    }

}
