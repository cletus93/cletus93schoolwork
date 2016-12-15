//This program reads from a file and displays every word that appears as well as how many times that word is in the document.
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char const *argv[])
{
	FILE *infile, *output;

	char word[2000], string[2000];
	int count, i, posn;

	infile = fopen ("proj1.in", "r");
	output = fopen ("proj1.out", "w");

	if (infile == NULL)
	{
		printf(File Missing");
		exit(-1);
	}

	int number_of_words =  fscanf(infile, "%s", word, posn);
	
	for (i = 0; i < number_of_words; i++)
	{
		char const *str = string;
			if(strcmp(string, word))
			count++;
		}
		fprintf (output, "%-s: %+d\n", word, count, posn);
		str += posn;
	}

	fclose(infile);
	fclose(output);
	return 0;
}
