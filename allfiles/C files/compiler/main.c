#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"

int main (int argc, char const *argv[])
{
   FILE *infile, *temp, *f;

   char inputstring[100];    //string from stdin
   char *resultptr;          //parsed string 
   int i, linecount = 0;

   if(argv[1] != NULL)    //checks if a file is passed
   {
      int i;
      int strsize = 0;
      for (i=1; i<argc; i++)
      {
         strsize += strlen(argv[i]);   //get filename length
         if (argc > i+1)
            strsize++;
      }
      char *inputfilename;
      char *asmfilename;

      inputfilename = malloc(strsize);
      inputfilename[0] = '\0';

      asmfilename = malloc(strsize);
      asmfilename[0] = '\0';

      for (i=1; i<argc; i++)
      {
         strcat(inputfilename, argv[i]); //add files name to filename
      }
      for (i=1; i<argc; i++)
      {
         strcat(asmfilename, argv[i]); //add files name to filename
      }
      strcat(inputfilename, ".fs16");   //add filename extension
      strcat(asmfilename, ".asm");

      if((f = fopen(asmfilename, "w+")) == NULL)
      {
         printf("Can't open input file.\n");
         exit(1);
      }

      if((infile = fopen(inputfilename, "r+")) == NULL)
      {
         printf("Can't open input file.\n");
         exit(1);
      }

      f = fopen (asmfilename, "w+");
      infile = fopen (inputfilename, "r+");
      resultptr = scannerparse(1, infile, 100, f);  //call to parse to each word
      fclose(infile);
      fclose(f);
   }
   else     //if no file passed
   {
      temp = fopen ("temp.fs16", "w+"); //use a temp file
      f = fopen("temp.asm", "w+");
      printf("Please enter your list of words and hit enter: \n");

      scanf("%[^\n]", inputstring);   //get list of words from stdin
      fprintf(temp, "%s", inputstring);   //print stdin to file
      fclose(temp);
      temp = fopen("temp.fs16", "r+");
      resultptr = scannerparse(2, temp, 100, f);    //call to parse file
      fclose(temp);
      fclose(f);
   }
   free(resultptr);
   return 0;
}

