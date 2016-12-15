//This program is designed to read all the characters from a file and then print them to stdout, nonprintable characters are printed as periods.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct word 
{
	char l[17];    
} word;

void listinsert(word *words, int *y, char *l);

int main(int argc, char *argv[])
{
	word words[17];
			
	FILE *input;
					
	unsigned char l[17];
	char infile[17];
								
	strlcpy(infile, argv[1], 17);
										
	int b = offsetof(struct word, l);
												
	input = fopen(argv[1], "rb");
														
	int y, i;
																
	if(input == 0)
	{
		printf("Please enter the right number of files.\n");
	}
	if(input != NULL)
	{
		y = 0;
		while(y = fread(l, 1, 16, input) > 0)
		{
			listinsert(words, &y, l);
			for(i = 0; i < y; i++) 
			{
				printf("%X: %X ", b, words[i].l);
				if(isprint(words[i].l))
				{
					printf("%c", words[i].l);
				}
				else
				{
					printf(".");
				}
				printf("\n");
			}
		}
	}
	fclose(input);

	return 0;
}

void listinsert(word *words, int *y, char *l)
{
	strcpy(words[*y].l, l);
	(*y)++;
}

