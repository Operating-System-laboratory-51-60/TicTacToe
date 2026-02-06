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
