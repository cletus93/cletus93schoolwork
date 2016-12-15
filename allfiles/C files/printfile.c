//This program reads a file and prints all numbers and characters removing the 0's.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
	if ( argc != 2 )
	{
		printf("Please re-run the program with a filename.");
	}
	else
	{
		FILE *infile;
		infile = fopen ( argv[1], "r" );

		if ( infile == 0 )
		{
			printf("Could not open file\n");
		}
		else
		{	
			int x;

			while ( ( x = fgetc(infile) ) != EOF )
			{
				printf("%02x ", x );
				printf("%c ", x );
			}

			fclose( infile );
		}
	}
}
