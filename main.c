#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 9

// ANSI color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int grid[N][N];
int original_grid[N][N];
int show_backtracking;

void print_grid() {
    printf("\033[2J");  // Clear the screen
    printf("\033[H");   // Cursor to home position

    for (int i = 0; i < N; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("------+-------+------\n");
        }
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            if (grid[i][j] == 0) {
                printf(". ");
            } else {
                if (original_grid[i][j] != 0) {
                    printf(ANSI_COLOR_RED "%d " ANSI_COLOR_RESET, grid[i][j]);
                } else {
                    printf(ANSI_COLOR_GREEN "%d " ANSI_COLOR_RESET, grid[i][j]);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

int is_valid(int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num) return 0;
        if (grid[x][col] == num) return 0;
    }
    
    int start_row = row - row % 3, start_col = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + start_row][j + start_col] == num) return 0;
    
    return 1;
}

int find_empty_location(int *row, int *col) {
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (grid[*row][*col] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

int solve_sudoku() {
    int row, col;

    if (!find_empty_location(&row, &col)) {
        return 1; // solved
    }

    for (int num = 1; num <= 9; num++) {
        if (is_valid(row, col, num)) {
            grid[row][col] = num;

            if (show_backtracking) {
                print_grid();
                usleep(1000); // 1ms
            }

            if (solve_sudoku()) {
                return 1;
            }

            grid[row][col] = 0;

            if (show_backtracking) {
                print_grid();
                usleep(1000); // 1ms
            }
        }
    }
    return 0;
}

int main() {

    while (1) {

        int load_from_file;

        printf("Load Sudoku from file? (1 for yes, 0 for no): ");
        scanf("%d", &load_from_file);

        if (load_from_file) {
            int test_number;
            printf("Enter test number (1, 2, 3, ...): ");
            scanf("%d", &test_number);

            char filename[100];
            sprintf(filename, "defaults/%d.txt", test_number);

            FILE *fp = fopen(filename, "r");
            if (fp == NULL) {
                printf("Error opening file %s!\n", filename);
                return 1;
            }

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    fscanf(fp, "%d", &grid[i][j]);
                }
            }

            fclose(fp);
        } else {
            printf("Enter the Sudoku puzzle (use 0 for empty cells):\n");
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    scanf("%d", &grid[i][j]);
                }
            }
        }

        // Copy the initial grid to original_grid
        memcpy(original_grid, grid, sizeof(grid));

        printf("Do you want to see the backtracking process? (1 for yes, 0 for no): ");
        scanf("%d", &show_backtracking);

        printf("\nOriginal Sudoku Grid:\n");
        print_grid();
        printf("Press Enter to start solving...");
        getchar(); // Consume newline
        getchar(); // Wait for Enter

        if (solve_sudoku()) {
            printf("\nSolved Sudoku:\n");
            print_grid();
        } else {
            printf("\nNo solution exists for this Sudoku puzzle.\n");
        }

    }

    return 0;
}