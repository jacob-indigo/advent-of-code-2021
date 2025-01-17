#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "inputs/04.h"

#define SIZE 5
#define N_BOARDS 100

typedef struct {
    int number;
    bool marked;
}BingoCell;

int markBoard(BingoCell *board, int draw) {
    for (int cell = 0; cell < (SIZE * SIZE); cell++) {
        if (board[cell].number != draw) continue;
        board[cell].marked = true;
    }

    return 0;
}

bool boardFinished(BingoCell *board) {
    // Check for complete column
    for (int col = 0; col < SIZE; col++){
        int marked_col_vals = 0;
        for (int cell = 0; cell < (SIZE * SIZE); cell++) {
            if (cell % SIZE != col) continue;
            if (!board[cell].marked) break;
            marked_col_vals += 1;

            if (marked_col_vals == SIZE) {
                return true;
            }
        }
    }

    // Check for complete row
    for (int row = 0; row < SIZE; row++) {
        int marked_row_vals = 0;
        for (int cell = 1; cell < ((SIZE * SIZE) + 1); cell++) {
            if (cell < row || cell > (row * SIZE)) continue;
            if (!board[cell - 1].marked) break;
            marked_row_vals += 1;

            if (marked_row_vals == SIZE) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    const unsigned char *s = input;
    int drawn_numbers[500] = {0};
    int n = 0;
    BingoCell boards[N_BOARDS][SIZE * SIZE];

    while (*s != '\0') {

        // Read in numbers to be drawn
        while (*s != '\n') {
            while (*s < '0' || *s > '9') s++;
            while (*s >= '0' && *s <= '9') {
                drawn_numbers[n] = (drawn_numbers[n] * 10) + (*s - '0');
                s++;
            }
            s++;
            n++;
        }
        printf("read in numbers (n=%d)\n", n);

        // Skip down to board descriptions
        s++;

        // Read in individual bingo boards
        for (int b = 0; b < N_BOARDS; b++) {

            BingoCell board[SIZE * SIZE];
            int value;
            for (int cell = 0; cell < (SIZE * SIZE); cell++) {
                value = 0;

                while (*s < '0' || *s > '9') s++;
                while (*s >= '0' && *s <= '9') {
                    value = (value * 10) + (*s - '0');
                    s++;
                }
                board[cell].number = value;
                board[cell].marked = false;
                s++;
            }

            // Add individual board to main array
            for (int c = 0; c < (SIZE * SIZE); c++) {
                boards[b][c] = board[c];
            }
        }

        break;
    }
    printf("read in boards\n");


    bool found_finished_board = false;
    int finished_board, winning_number;
    for (int i = 0; i < n; i++) {
        printf("\nCalled %d\n", drawn_numbers[i]);
        for (int board_i = 0; board_i < N_BOARDS; board_i++) {
            markBoard(boards[board_i], drawn_numbers[i]);

            if (boardFinished(boards[board_i])) {
                found_finished_board = true;
                finished_board = board_i;
                for (int z=0; z < 25; z++){
                    printf("%d (%d)\n", boards[board_i][z].number, boards[board_i][z].marked);
                }
                break;
            }
        }
        /*
        for (int i=0; i < 25; i++){
            printf("%d (%d)\n", boards[2][i].number, boards[2][i].marked);
        }
        */

        if (found_finished_board) {
            winning_number = drawn_numbers[i];
            break;
        }
    }
    printf("found winning board\n");

    // Sum unmarked numbers of selected board
    int unmarked = 0;
    for (int cell = 0; cell < (SIZE * SIZE); cell++) {
        if (boards[finished_board][cell].marked) continue;
        unmarked += boards[finished_board][cell].number;
    }
    printf("summing unmarked\n");

    printf("\n%d", unmarked * winning_number);

    return 0;
}
