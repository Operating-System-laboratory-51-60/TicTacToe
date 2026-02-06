
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>   
#include <string.h>   

volatile int signal_count = 0;
volatile int move_received = 0; 
char board[9] = {'1','2','3','4','5','6','7','8','9'}; // Game board


void counter_increment_handler(int sig)
{
	(void)sig;
	signal_count++;
}

void move_receive_check_handler(int sig)
{
	(void)sig;
	move_received = 1;
}


void print_board()
{
#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#else
	system("clear");
#endif
	printf("\n");
	printf(" %c | %c | %c \n", board[0], board[1], board[2]);
	printf("---|---|---\n");
	printf(" %c | %c | %c \n", board[3], board[4], board[5]);
	printf("---|---|---\n");
	printf(" %c | %c | %c \n", board[6], board[7], board[8]);
	printf("\n");
}


int check_win()
{
	int wins[8][3] = {
		{0,1,2}, {3,4,5}, {6,7,8},
		{0,3,6}, {1,4,7}, {2,5,8},
		{0,4,8}, {2,4,6}
	};

	for (int i = 0; i < 8; i++) {
		int a = wins[i][0], b = wins[i][1], c = wins[i][2];
		if (board[a] == board[b] && board[b] == board[c])
			return 1;
	}
	return 0;
}

int main()
{
	pid_t my_pid = getpid();    // This process' PID
	pid_t opp_pid;              
	char my_symbol, opp_symbol; // 'X' or 'O'
	int my_turn = 0;            // 1 if it's this process' turn

	// Setup signal handlers
	signal(SIGUSR1, counter_increment_handler);   // Increment counter per signal
	signal(SIGUSR2, move_receive_check_handler);  // Mark move as complete

	printf("--- TERMINAL TIC TAC TOE ---\n");
	printf("My PID is: %d\n", (int)my_pid);
	printf("Enter Opponent's PID: ");
	int tmp_pid;
	if (scanf("%d", &tmp_pid) != 1) {
		fprintf(stderr, "Invalid PID input.\n");
		return 1;
	}
	opp_pid = (pid_t)tmp_pid;

	if(my_pid == opp_pid)
    {
        printf("\n\n");
        printf("Error: You cannot play against yourself!\n");
        return 0;
    }

    // whose pid is smaller goes first
    if (my_pid < opp_pid)
    {
        printf("I go FIRST (X).\n");
        my_turn = 1;
        my_symbol = 'X';
        opp_symbol = 'O';
    }
    else
    {
        printf("I go SECOND (O).\n");
        my_turn = 0;
        my_symbol = 'O';
        opp_symbol = 'X';
    }
    sleep(1);
	int moves_count = 0;
	while (moves_count < 9) {
		print_board();

		if (check_win())
			break;

		if (my_turn) {
			int move;
			printf("Your Turn (%c). Enter position (1-9): ", my_symbol);
			if (scanf("%d", &move) != 1) {
				printf("Invalid input. Try again.\n");
				// clear stdin
				int ch;
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				sleep(1);
				continue;
			}

			int idx = move - 1; // moves are 1-indexed

			if (idx < 0 || idx > 8 || board[idx] == 'X' || board[idx] == 'O') {
				printf("Invalid move!\n");
				sleep(2);
				continue;
			}

			board[idx] = my_symbol;
			moves_count++;
			print_board();

			if (check_win()) {
				printf("YOU WON!\n");
			}

			// Send opponent signals encoding the position
			printf("Sending move to opponent...\n");
			// Send SIGUSR1 (idx+1) times; opponent will count signals
			for (int i = 0; i < idx + 1; i++) {
				kill(opp_pid, SIGUSR1);
				usleep(50000); // 50ms between signals to reduce merging
			}

			// Indicate end of transmission
			kill(opp_pid, SIGUSR2);

			my_turn = 0;
			if (check_win())
				break;
		} else {
			printf("Waiting for opponent (%c)...\n", opp_symbol);

			// Reset counters and wait until opponent signals end of move
			signal_count = 0;
			move_received = 0;

			while (move_received == 0) {
				sleep(1);
			}

			int opponent_idx = signal_count - 1;
			if (opponent_idx >= 0 && opponent_idx <= 8)
				board[opponent_idx] = opp_symbol;
			moves_count++;

			if (check_win()) {
				print_board();
				printf("YOU LOST.\n");
				break;
			}

			my_turn = 1;
		}
	}

	if (!check_win() && moves_count == 9) {
		print_board();
		printf("IT'S A DRAW!\n");
	}

	return 0;
}