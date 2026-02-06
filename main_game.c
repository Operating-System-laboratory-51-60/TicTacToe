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
//logic for current player's turn and sending signal to opponent
board[idx] = my_symbol;
            moves_count++;
            print_board();

            if (check_win()) {
                printf("YOU WON!\n");
                // Send dummy moves to let opponent receive 'something' then they will check board state
                // But actually, we just need to wake them up. 
                // We send the move so they can update their board and see they lost.
            }

            // Mechanism to wake the opponent via signals
            printf("Sending move to opponent...\n");
            // Send SIGUSR1 'idx + 1' times because is idx is 0 then it is impossible to send
            for(int i=0;i<idx+1;i++)
            {
                kill(opp_pid,SIGUSR1);
                usleep(50); // sleeping for 1 second to ensure signals do not merge
            }

            // sending signal that i am done
            kill(opp_pid,SIGUSR2);

            my_turn = 0;
            if(check_win()) break; // just for safety
        }