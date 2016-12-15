//This program reads from a file and prints all the characters to the screen, it prints nonprintable characters as periods.
#include <stdio.h>
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
			int m, n, i, j, k, count = 0, printcount = 16;
			unsigned char myarr[20];
			
			while ( ( n = fgetc(infile) ) != EOF)
			{
				myarr[count] = n;
				count++;
			}
			for (i = 0; i < count; i++)
			{
				if ( i % 16 == 0)
				{
					if ( i > 0)
					{
						printf("\n");
					}
					printf("%04x: ", i);
				}
				printf("%02x ", myarr[i]);
				if ( i % 15 == 0)
				{
					if ( i > 0)
					{
						m = i - printcount;
						for (j = 0; j <= i; j++)
						{
							if ( myarr[m] != 0)
							{
								if ( myarr[m] < 0x7e || myarr[m] > 0x20)
								{
								printf("%c ", myarr[m]);
								printcount++;
								}
								else
								{
									printf(". ");
								printcount++;
								}
							}	
							m++;	
						}

					}
				}
			}
			 k = i - m;
			 m = i - k;
			 for (j = 0; j <= i; j++)
			 {
				 if ( myarr[m] != 0)
				 {
					 if ( myarr[m] < 0x7e || myarr[m] > 0x20)
					 {
						 printf("%c ", myarr[m]);
						 printcount++;
					 }
					 else
					 {
						 printf(". ");
						 printcount++;
					 }
				 }
				 m++;
			 }

			printf("\n");
			fclose( infile );
		}
	}
return 0;
}
