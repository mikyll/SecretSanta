/*
*	This software is provided 'as-is', without any express or implied
*	warranty. In no event will the authors be held liable for any damages
*	arising from the use of this software.
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute it
*	freely.
*
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VERSION "0.3"
#define MAX_PARTICIPANTS 100
#define MAX_NAME_LENGTH 64

int duplicateCheck(char lista[MAX_PARTICIPANTS][MAX_NAME_LENGTH], int n);
int invalidSantaCheck(int lista[MAX_PARTICIPANTS], int n);
void printSanta(char listP[MAX_PARTICIPANTS][MAX_NAME_LENGTH], int listS[MAX_PARTICIPANTS], int n);

int main(int argc, char ** argv)
{
	printf("SECRET SANTA v%s\n\n\n", VERSION);

	// parameters check
	if (argc != 1)
	{
		return -1;
	}

	// INIZIALIZATION
	char participants[MAX_PARTICIPANTS][MAX_NAME_LENGTH]; // participants list
	int secretSanta[MAX_PARTICIPANTS]; // contains the results (secretSanta[giverIndex] = receiverIndex)
	
	char input[MAX_NAME_LENGTH];
	int i, j, n_participants = 0;
	
	srand(time(NULL)); // seed inizialization

	// array filled with result index. Remember: "secretSanta[giver] = receiver"
	for (i = 0; i < n_participants; i++)
		secretSanta[i] = i;

	// MENU
	do
	{
		printf("Enter the names of the participants ('ok' to finish, 'exit' to quit):\n");

		for (i = 0, n_participants = 0; i < MAX_PARTICIPANTS; i++)
		{
			char tmp_name[MAX_NAME_LENGTH];

			printf("%d) ", i + 1);
			scanf("%s", &tmp_name);

			if (strcmp(tmp_name, "ok") == 0 || strcmp(tmp_name, "OK") == 0 || tmp_name[0] == '\0')
				break;
			if (strcmp(tmp_name, "exit") == 0 || strcmp(tmp_name, "EXIT") == 0 || strcmp(tmp_name, "quit") == 0 || strcmp(tmp_name, "QUIT") == 0)
				return 0;

			strcpy(participants[i], tmp_name);
			n_participants++;
		}
		

		// CHECK LIST ERRORS
		if (n_participants < 4)
		{
			printf("\nERROR: Need at least 4 participants\n\n");
			continue;
		}
		if (duplicateCheck(participants, n_participants) != 0)
		{
			printf("\nERROR: One or more names have a duplicate\n\n");
			continue;
		}

		// LIST CONFIRMATION
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPARTICIPANTS LIST SUMMARY:\n");
		for (i = 0; i < n_participants; i++)
		{
			printf("%d) %s\n", i + 1, participants[i]);
		}

		printf("\nConfirm to proceed? [y/n]\n");
		scanf("%s", input);
	} while (input[0] != 'y' && input[0] != 'Y' && strcmp(input, "ok") != 0 && strcmp(input, "OK") != 0);

	
	// RANDOMIZE ALGORITHMS
	// 1st algorithm (SIMPLE BUT NOT MUCH SCALABLE)
	// +---------------------------------------------------------------------------------+
	// | Shuffle the array containing the receivers indexes, then check for "errors" (a  |
	// | person cannot give a present to himself). In this case it shuffle the whole     |
	// | array again, until it gets it right. As it is random, and has to be redone      |
	// | completely everytime, the more the number of participants grows, the more       |
	// | likely it is that at least one element is invalid.                              |
	// +---------------------------------------------------------------------------------+
	//
	/*do
	{
		// shuffle secretSanta array
		for (i = 0; i < n_participants; i++)
		{
			int temp = secretSanta[i];
			int randomIndex = rand() % n_participants;

			secretSanta[i] = secretSanta[randomIndex];
			secretSanta[randomIndex] = temp;
		}
		// check duplicates
	} while (invalidSantaCheck(secretSanta, n_participants) >= 0);
	*/


	// 2nd algorithm
	// +---------------------------------------------------------------------------------+
	// | Shuffle the array once, then checks if there are "errors" (the receiver index   |
	// | is the same of the giver). In this case it cycles, swapping that index with a   |
	// | random one, until it gets a full valid array.                                   |
	// +---------------------------------------------------------------------------------+
	//
	// shuffle secretSanta array
	/*for (i = 0; i < n_participants; i++)
	{
		int temp = secretSanta[i];
		int randomIndex = rand() % n_participants;

		secretSanta[i] = secretSanta[randomIndex];
		secretSanta[randomIndex] = temp;
	}
	while ((i = invalidSantaCheck(secretSanta, n_participants)) >= 0)
	{
		int new_i, old_i;
		while((new_i = rand() % n_participants) == i || secretSanta);
		old_i = i;
		secretSanta[old_i] = new_i;
		secretSanta[new_i] = old_i;
	}
	// UPGRADE: the function 'invalidSantaCheck' gives the array of the incorrect index, so it does just one cycle to check if it's valid.
	*/
	
	
	// 3rd Algorithm
	// +---------------------------------------------------------------------------------+
	// | Everytime a new element has to be added to the secretSanta array, it makes a    |
	// | CHECK: if the random index has been already used (or if it is the same of the   |
	// | receiver, it isn't valid and it has to be choosen again).                       |
	// | This solution uses a "support array" to check if an index has been already used |
	// | (if the array value at the index is -1, then it hasn't been used yet).          |
	// +---------------------------------------------------------------------------------+
	//
	// fills an array with -1
	int alreadyUsed[MAX_PARTICIPANTS]; // array which tells if an index has been already used (-1 not used, 0 used)
	for (i = 0; i < n_participants; i++)
		alreadyUsed[i] = -1;
	
	for (i = 0; i < n_participants; i++)
	{
		int random; // index of the giver ('i' is the index of the receiver)

		// checks that the person who's giving isn't the same who is receiving OR someone is already giving that person something
		for (j = 0; (random = rand() % n_participants) == i || alreadyUsed[random] != -1; j++)
		{
			if (i == n_participants - 1 && j == n_participants / 2) // means we're in a deadlock (the only one not used index is the last)
			{
				random = rand() % n_participants - 1;
				
				int oldValue = secretSanta[random];
				secretSanta[random] = i;
				
				random = oldValue;
				break;
			}
		}

		alreadyUsed[random] = 0;
		secretSanta[i] = random;
	}
	


	// 3rd algorithm (VARIANT)
	// if in the last cycle the only index remained is the same number of the cycle,
	// it keeps going back of 1 cycle until it gets it right.
	//
	/*int TMP[MAX_PARTICIPANTS];
	int skip = -1;
	for (i = 0; i < n_participants; i++)
		TMP[i] = -1;

	TMP[2] = 0, TMP[0] = 0, TMP[1] = 0;
	secretSanta[0] = 2, secretSanta[1] = 0, secretSanta[2] = 1;

	for (i = 3; i < n_participants; i++)
	{
		int random; // random --gives to--> i
		int oldRandom = 1;

		// checks that the person who's giving isn't the same who is receiving OR someone is already giving that person something
		for (j = 0; (random = rand() % n_participants) == i || TMP[random] != -1; j++)
		{
			if (i == n_participants - 1 && j == n_participants / 2) // means we're in a deadlock (the only one not used index is the last)
			{
				i = i - 2; // go back of one cycle
				TMP[oldRandom] = -1;
				skip = 0;
				break;
			}
			printf("i=%d, random=%d\n", i, random);
		}

		if (skip == 0)
		{
			skip = -1;
			continue;
		}
		TMP[random] = 0;
		secretSanta[i] = random;
		
		oldRandom = random;
	}
	*/

	// PRINT SECRET SANTA
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSECRET SANTA:\n");
	printSanta(participants, secretSanta, n_participants);


	// WRITE RESULT TO FILE
	printf("\n\nWrite the results on file? [y/n]\n");
	scanf("%s", input);
	if (input[0] == 'y' || input[0] == 'Y' || strcmp(input, "ok") == 0 || strcmp(input, "OK") == 0)
	{
		FILE *f;

		if ((f = fopen("SecretSanta.txt", "w")) == NULL)
		{
			printf("Errore nella creazione del file.\n");
			return -1;
		}

		for (i = 0; i < n_participants; i++)
		{
			fprintf(f, "%s\tmakes a gift for %s\n", participants[i], participants[secretSanta[i]]);
		}

		fclose(f);
	}

	printf("\nPress 'ENTER' to quit\n");
	getchar();
	getchar();
	return 0;
}


// FUNCTIONS
int duplicateCheck(char list[MAX_PARTICIPANTS][MAX_NAME_LENGTH], int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (strcmp(list[i], list[j]) == 0)
				return -1;
		}
	}
	return 0;
}

int invalidSantaCheck(int list[MAX_PARTICIPANTS], int n) // returns -1 if it's valid; otherwise it returns the first index of the invalid one
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (list[i] == i)
			return i;
	}
	return -1;
}

void printSanta(char listP[MAX_PARTICIPANTS][MAX_NAME_LENGTH], int listS[MAX_PARTICIPANTS], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		printf("%s\tmakes a present for %s\n", listP[i], listP[listS[i]]);
	}
}
