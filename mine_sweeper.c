#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

char * init_board(int num_rows, int num_columns, char symbol){
    int i=0, j=0;
    char *board = (char *)malloc(sizeof(char) * num_columns * num_rows);
    for(i=0; i<num_rows; i++)
        for(j=0; j<num_columns; j++)
            board[i * num_columns + j]=symbol;
    return board;
}

void print_value(char *board, int num_rows, int num_columns){
    //printf("print values\n");
    int i,j;
    for(i=num_rows-1; i>=0; i--){
        printf("%d ", i);
        for(j=0; j<num_columns-1; j++)
            printf("%c ", board[i * num_columns + j]);
        printf("%c\n", board[i * num_columns + j]);
    }
    // last line
    printf("  ");
    for(i=0; i<num_columns-1; i++)
        printf("%d ",i);
    printf("%d\n",i);
}

void print_status(board_t board){
    int i,j;
    int num_rows = board.row;
    int num_columns = board.col;
    printf("There are %d mines left\n", *board.mine_left);
    for(i=num_rows-1; i>=0; i--){
        printf("%d ", i);
        for(j=0; j<num_columns-1; j++){
            if(board.status[i * num_columns + j]=='r') //r means revealed
                printf("%c ", board.values[i * num_columns + j]);
            else
                printf("%c ", board.status[i * num_columns + j]);
        }
        if(board.status[i * num_columns + j]=='r')
            printf("%c\n", board.values[i * num_columns + j]);
        else
            printf("%c\n", board.status[i * num_columns + j]);

    }
    // last line
    printf("  ");
    for(i=0; i<num_columns-1; i++)
        printf("%d ",i);
    printf("%d\n",i);
}

void place_mine(board_t *board){
    int rand_row=0;
    int rand_col=0;
    int counter=0;
    //printf("seed is %d", board->seed);
    srand(board->seed);
    while(1){
        rand_col = rand() % board->col;
        rand_row = rand() % board->row;

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


void recsive_reveal(board_t board, int action_row, int action_col){


    //printf("current: %d, %d\n", action_row, action_col);
    if(action_col<0 || action_row<0 || action_row>=board.row || action_col >= board.col){ //terminated
        //printf("return because terminated\n");
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

void make_action(board_t board, int action_row, int action_col, char action){

    board.status[action_row * board.col + action_col] = action;

    if(action == '!'){
        (*board.mine_left)--;
        if(*board.mine_left<0)
            *board.mine_left=0;
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
            if(action == 0)
                board.status[action_row * board.col + action_col] = '#';
            else
                continue;
            print_status(board);
        }


    }

}
