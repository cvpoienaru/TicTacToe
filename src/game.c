/**
 * Copyright (c) 2015, Codrin-Victor Poienaru.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * This software is provided by the copyright holders and contributors "as is"
 * and any express or implied warranties, including, but not limited to, the
 * implied warranties of merchantability and fitness for a particular purpose
 * are disclaimed. In no event shall the copyright holder or contributors be
 * liable for any direct, indirect, incidental, special, exemplary, or
 * consequential damages (including but not limited to, procurement of
 * substitute goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether in
 * contract, strict liability, or tort (including negligence or otherwise)
 * arising in any way out of the use of this software, even if advised of the
 * possibility of such damage.
 */

#include <stdlib.h>
#include <math.h>
#include "game.h"

int* alloc_board(void)
{
    int* board = (int*)malloc(pow(BOARD_SIZE, 2) * sizeof(int));
    return !board ? NULL : board;
}

void free_board(int** board)
{
    if(!board || !(*board))
        return;

    free(*board);
    *board = NULL;
}

int init_board(int** board)
{
    int i, j;

    if(!board || !(*board))
        return FALSE;

    for(i = 0; i < BOARD_SIZE; ++i) {
        for(j = 0; j < BOARD_SIZE; ++j) {
            (*board)[i * BOARD_SIZE + j] = FREE;
        }
    }

    return TRUE;
}

int move_board(int** board, int row, int column, int symbol)
{
    if(!board || !(*board))
        return FALSE;
    if(row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
        return FALSE;
    if((*board)[row * BOARD_SIZE + column] != FREE)
        return FALSE;

    (*board)[row * BOARD_SIZE + column] = symbol;
    return TRUE;
}

int check_board(int* board)
{
    int result = FALSE;
    int i, j, dimensions;
    int free_cell = FALSE;
    int* winner = NULL;

    if(!board)
        goto exit;

    dimensions = 2 * BOARD_SIZE + 2;
    winner = (int*)calloc(dimensions, sizeof(int));
    if(!winner)
        goto exit;

    for(i = 0; i < BOARD_SIZE; ++i) {
        winner[dimensions - 1] += board[i * BOARD_SIZE + i];
        winner[dimensions - 2] += board[i * BOARD_SIZE + (BOARD_SIZE - i - 1)];
        for(j = 0; j < BOARD_SIZE; ++j) {
            if(board[i * BOARD_SIZE + j] == FREE) {
                free_cell = TRUE;
            }

            winner[i] += board[i * BOARD_SIZE + j];
            winner[BOARD_SIZE + j] += board[i * BOARD_SIZE + j];
        }
    }

    for(i = 0; i < dimensions; ++i) {
        if(winner[i] == BOARD_SIZE * PLAYER_A) {
            result = PLAYER_A;
            goto exit;
        }
        if(winner[i] == BOARD_SIZE * PLAYER_B) {
            result = PLAYER_B;
            goto exit;
        }
    }

    if(free_cell == FALSE)
        result = DRAW;
exit:
    if(winner)
        free(winner);
    return result;
}