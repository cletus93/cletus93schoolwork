//Randall Rootz, 9/17/16, This program takes either a list of integers from the user or a list of integers from a file and sorts them using a tree from least to greatest.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "traversals.h"
#include "buildTree.h"

int main (int argc, char const *argv[])
{
   FILE *infile, *outfile1, *outfile2, *outfile3, *temp;

   char filename1[] = "out.inorder", filename2[] = "out.preorder", filename3[] = "out.postorder";
   int i, n, count = 0;
   int arr[1000];

   if(argv[1] != NULL)//checks if a file is passed
   {
      int i;
      int strsize = 0;
      for (i=1; i<argc; i++)
      {
         strsize += strlen(argv[i]);//get filename length
         if (argc > i+1)
            strsize++;
      }
      char *newfilename1, *newfilename2, *newfilename3, *inputfilename;

      newfilename1 = malloc(strsize);//allocate memory length to strings
      newfilename2 = malloc(strsize);
      newfilename3 = malloc(strsize);
      inputfilename = malloc(strsize);

      newfilename1[0] = '\0';//add end of line
      newfilename2[0] = '\0';
      newfilename3[0] = '\0';
      inputfilename[0] = '\0';

      for (i=1; i<argc; i++)
      {
         strcat(newfilename1, argv[i]);//add filename from command line to strings
         strcat(newfilename2, argv[i]);
         strcat(newfilename3, argv[i]);
         strcat(inputfilename, argv[i]);
      }

      strcat(newfilename1, ".inorder");//add extensions to input and output filenames
      strcat(newfilename2, ".preorder");
      strcat(newfilename3, ".postorder");
      strcat(inputfilename, ".data");

      if((infile = fopen(inputfilename, "r")) == NULL)
      {
         printf("Can't open input file.\n");
         exit(1);
      }

      infile = fopen (inputfilename, "r");

      while(fscanf(infile, "%d", &n) == 1)//read from inputfile
      {
         arr[count] = n;
         count++;
      }
      strcpy(filename1, newfilename1);
      strcpy(filename2, newfilename2);
      strcpy(filename3, newfilename3);
          fclose(infile);
   }
   else
   {
      temp = fopen ("temp.data", "wb");

      printf("Please enter you list of integers, then type go: ");

      while(scanf("%d", &n) == 1)//get keyboard input
      {
         fprintf(temp, "%d ", n);//print input to temp file
      }
      fclose(temp);
      temp = fopen ("temp.data", "r");
      while(fscanf(temp, "%d", &n) == 1)//read input from temp file
      {
         arr[count] = n;
         count++;
      }
      fclose(temp);
   }

   outfile1 = fopen (filename1, "wb");
   outfile2 = fopen (filename2, "wb");
   outfile3 = fopen (filename3, "wb");

   struct treenode *bt;//construct new tree
   bt = NULL;

   for ( i = 0; i < count; i++)
   {
      insert ( arr[i], &bt, 0);//insert all integers to the tree
   }

   inorder ( bt );//sort tree by inorder
   fprintinordertree(bt, outfile1);//print tree by inorder to file

   preorder ( bt );//sort tree by preorder
   fprintpreordertree(bt, outfile2);//print tree by preorder to file

   postorder ( bt );//sort tree by postorder
   fprintpostordertree(bt, outfile3);//print tree by postorder to file

   fclose(outfile1);
   fclose(outfile2);
   fclose(outfile3);
   return 0;
}
