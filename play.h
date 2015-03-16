#ifndef PLAY_H
#define PLAY_H

void make_action(board_t board, int action_row, int action_col, char action);
int win(board_t board, int row, int col);
void recsive_reveal(board_t board, int action_row, int action_col);

#endif
