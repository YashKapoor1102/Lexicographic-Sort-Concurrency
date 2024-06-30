/*
Name: Yash Kapoor
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#include <sys/shm.h>
#include "CSORT.h"

#include <sys/sem.h>

#include "semun.h"

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main(int argc, char *argv[]) 
{
	int letters;
	int shmid;
	int found;
	char temp[256];
	char letter;
	char store;

	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	// create semaphore 

	// initializing the semaphore
	if (!set_semvalue()) {
		fprintf(stderr, "Failed to initialize semaphore\n");
		exit(EXIT_FAILURE);
	}
	sleep(2);

	pid_t pid, pid2;

	void *shared_memory = (void *) 0;
	struct shared_use_st *shared_stuff;

	printf("\nSort Array into Lexicographic Order: Program Starting\n");

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	// create shared memory

	if (shmid == -1) {
		// error occurs
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// making shared memory accessible to the program
	shared_memory = shmat(shmid, (void *) 0, 0);

	if (shared_memory == (void *) -1) {
		// error occurs
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	printf("\n\nMemory attached at %X\n", (intptr_t) shared_memory);

	// assigning the shared_memory segment to shared_stuff
	shared_stuff = (struct shared_use_st *) shared_memory;

	for(int initialize = 0; initialize < 7; initialize++) {
		// initializing the array with empty character values
		shared_stuff->ar[initialize] = ' ';
	}
		
	for(letters = 0; letters < 7; letters++) {
		found = 0;

		// asking the user for seven distinct letters
		printf("Enter letter %d: ", (letters + 1)); 
		
		if (fgets(temp, sizeof(temp), stdin) == NULL) {
			printf("Input error.\n");
		}

		letter = temp[0];
		// if user enters more than one letter accidentally
		// only the first letter will be read and all other letters are ignored

		for(int checkDuplicates = 0; checkDuplicates < 7; checkDuplicates++) {
			// iterating through the array that contains 7 distinct letters
			// to check if the array contains the letter
			if(shared_stuff->ar[checkDuplicates] == tolower(letter)) {
				// letter is found
				found = 1;
				break;
			}
		}

		if(found) {
			printf("\nSorry! You did not enter a distinct letter. Try again!\n\n");
			letters--;
		}
		else if (!isalpha(letter)) {
			printf("\nSorry! You must enter an alphabet. Try again!\n\n");
			letters--;
		}
		else {
			shared_stuff->ar[letters] = tolower(letter);
		}
	}
	printf("Unsorted List: ");
	for (int i = 0; i < 7; i++) {
		printf(" %c", shared_stuff->ar[i]); fflush(stdout);
	}
	printf("\n");

	char debugMode;
	printf("Do you want to run this program in debug mode? (Y/N): ");
	scanf(" %c", &debugMode);
	printf("\n");

	// Creating 2 child processes from a single parent
	// So, we have a total of 3 concurrent processes running (2 children, 1 parent = 3)
	(pid = fork()) && (pid2 = fork());

	if ((pid == -1) || (pid2 == -1)) {
		// error occurs while forking
		perror("fork failed");
		exit(1);
	}
	else if (pid == 0) {		// Process 1

		if (!semaphore_p()) exit(EXIT_FAILURE);
		// acquiring the semaphore 
		
		printf("\n");

		int j;
		for(int i = 1; i < 3; i++) {
			// sorting first, second, and third letter in lexicographic order
			// if needed
			int check = 0;
			store = shared_stuff->ar[i];
			j = i - 1;
			while(j >= 0 && shared_stuff->ar[j] > store) 	{
				// swapping is performed if this while-loop runs
				/*
				if two letters are not in lexicographic order, 
				then swap those two letters 
				*/
				check = 1;
				shared_stuff->ar[j + 1] = shared_stuff->ar[j];
				j--;
			}
			if(check == 1 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P1: Performed Swapping");
			}
			if(check == 0 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P1: No Swapping");
			}
			shared_stuff->ar[j + 1] = store;

		}

		if (!semaphore_v()) exit(EXIT_FAILURE);
		// releasing the semaphore
		
				
	}
	else if (pid2 == 0) {		// Process 2
		sleep(1);
		// sleeps for 1 second, so process 1 can acquire the semaphore first

		if (!semaphore_p()) exit(EXIT_FAILURE);
		// acquiring the semaphore

		printf("\n");

		int j;
		for(int i = 3; i < 5; i++) {
			// sorting third, fourth, and fifth letter in lexicographic order in the 
			// the correct position of the list, if needed
			int check = 0;
			store = shared_stuff->ar[i];
			j = i - 1;
			while(j >= 0 && shared_stuff->ar[j] > store) 	{
				check = 1;
				shared_stuff->ar[j + 1] = shared_stuff->ar[j];
				j--;
			}

			if(check == 1 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P2: Performed Swapping");
			}
			if(check == 0 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P2: No Swapping");
			}

			shared_stuff->ar[j + 1] = store;

		}

		if (!semaphore_v()) exit(EXIT_FAILURE);
		// releasing the semaphore
	}
	else {			// Process 3
		sleep(2);
		// sleeps for 2 seconds so process 1 can acquire the semaphore first

		if (!semaphore_p()) exit(EXIT_FAILURE);
		// acquiring the semaphore

		printf("\n");

		int j;
		for(int i = 5; i < 7; i++) {
			// sorting fifth, sixth, and seventh letter in lexicographic order
			// in the correct position of the list, if needed
			int check = 0;

			store = shared_stuff->ar[i];
			j = i - 1;
			while(j >= 0 && shared_stuff->ar[j] > store) 	{
				check = 1;
				shared_stuff->ar[j + 1] = shared_stuff->ar[j];
				j--;
			}
			if(check == 1 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P3: Performed Swapping");
			}
			if(check == 0 && (debugMode == 'Y' || debugMode == 'y')) {
				printf("\nProcess P3: No Swapping");
			}

			shared_stuff->ar[j + 1] = store;

		}

		// By the end of this process, the entire list will be sorted in lexicographic 			// order

		printf("\n");
		printf("\nList in lexicographic order: ");
		for(int j = 0; j < 7; j++) {
			printf("%c ", shared_stuff->ar[j]);	
		}
		
		printf("\n");

		if (!semaphore_v()) exit(EXIT_FAILURE);
		// releasing the semaphore

		sleep(5);
		// allowing the parent to sleep for 5 seconds
		// to ensure all processes are complete 
		// before deleting the semaphore and shared memory

		// Shared memory is detached and then deleted 
		if(shmdt(shared_memory) == -1) {
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			fprintf(stderr, "shmctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}

		del_semvalue();
		exit(EXIT_SUCCESS);

	}			

}

static int set_semvalue(void) 
{
	union semun sem_union;
	
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
	
	return (1);
}

static void del_semvalue(void) 
{
	union semun sem_union;

	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(void) 
{
	// "wait" operation

	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1; 	/* P() */
	sem_b.sem_flg = SEM_UNDO;
	
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return (1);
}

static int semaphore_v(void)  
{
	// "release" operation, so the semaphore becomes available
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1;	/* V() */
	sem_b.sem_flg = SEM_UNDO;
	
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return(0);
	}
	
	return(1);
}

