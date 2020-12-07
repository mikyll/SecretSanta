#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VERSION 1
#define MAX_PARTECIPANTS 100
#define MAX_NAME_LENGTH 64

int duplicateCheck(char lista[MAX_PARTECIPANTS][MAX_NAME_LENGTH], int n);

int main(int argc, char ** argv)
{
	printf("SECRET SANTA v%d\n\n\n", VERSION);

	// parameters check
	if (argc != 1)
	{
		return -1;
	}

	// INIZIALIZATION
	char partecipants[MAX_PARTECIPANTS][MAX_NAME_LENGTH];
	char input[8];
	int i, n_partecipants = 0, ok;
	int secretSanta[MAX_PARTECIPANTS];

	srand(time(NULL)); // seed inizialization

	// MENU    
	do
	{
		printf("Enter partecipant names ('OK' to finish):\n");

		for (i = 0; i < MAX_PARTECIPANTS; i++)
		{
			char tmp_name[MAX_NAME_LENGTH];

			printf("%d) ", i + 1);
			scanf("%s", &tmp_name);

			if (strcmp(tmp_name, "OK") == 0 || strcmp(tmp_name, "ok") == 0 || tmp_name[0] == '\0')
				break;

			strcpy(partecipants[i], tmp_name);
			n_partecipants++;
		}

		// CHECK LIST ERRORS
		if (n_partecipants < 4)
		{
			printf("\nERROR: Need at least 4 partecipants\n\n");
			continue;
		}
		if (duplicateCheck(partecipants, n_partecipants) != 0)
		{
			printf("\nERROR: One or more names have a duplicate\n\n");
			continue;
		}

		// LIST CONFIRMATION
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPARTECIPANTS LIST SUMMARY:\n");
		for (i = 0; i < n_partecipants; i++)
		{
			printf("%d) %s\n", i + 1, partecipants[i]);
		}

		printf("\nConfirm to proceed? [y/n]\n");
		scanf("%s", input);
	} while (input[0] != 'y' && input[0] != 'Y');


	// RANDOMIZE    
	// fill an array with partecipant indexes
	for (i = 0; i < n_partecipants; i++)
		secretSanta[i] = i;

	// shuffle array (skipping same indexes - CARE, IT DOESN'T SCALE!)
	do
	{
		for (i = 0, ok = 1; i < n_partecipants; i++)
		{
			int temp = secretSanta[i];
			int randomIndex = rand() % n_partecipants;

			if (temp == randomIndex)
			{
				ok = 0;
				break;
			}
			secretSanta[i] = secretSanta[randomIndex];
			secretSanta[randomIndex] = temp;
		}
	} while (!ok);

	// SECRET SANTA
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSECRET SANTA:\n");
	for (i = 0; i < n_partecipants; i++)
	{
		printf("%s\t must make a gift to %s\n", partecipants[i], partecipants[secretSanta[i]]);
	}

	getchar();
	getchar();
	return 0;
}

int duplicateCheck(char lista[MAX_PARTECIPANTS][MAX_NAME_LENGTH], int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (strcmp(lista[i], lista[j]) == 0)
				return -1;
		}
	}
	return 0;
}