// Importing necessary libraries
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h> // For catching signals from other terminals
#include<string.h> // For printing the board
#include<sys/types.h>

// Global variables
// We are using volatile because these variables will change as the game goes on
// Without these the compiler will try to optimize and keep these variables in the cache
// Using volatile we are telling the compiler that these can change in values and every time fetch them from ram
volatile int signal_count = 0; //
volatile int move_received = 0; // Flag to indicate a full move has arrived
char board[9] = {'1','2','3','4','5','6','7','8','9'}; // The board which we will modify as the game goes on


















































































int moves_count = 0;
while(moves_count < 9)
{
    // print the board every 9 iteration
    print_board();

    if(check_win()) break; // is current board state is a winning state then break

    if(my_turn)
    {
        int move;
        printf("Your Turn (%c). Enter position (1-9): ", my_symbol);
        scanf("%d", &move);
        int idx = move - 1; // moves are 1 indexed and array is 0 indexed

        if (idx < 0 || idx > 8 || board[idx] == 'X' || board[idx] == 'O')
        {
            // These are all invalid checks
            printf("Invalid move!\n");
            sleep(2);
            continue;
        }