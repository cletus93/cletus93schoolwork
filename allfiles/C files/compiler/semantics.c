#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "compilertree.h"

char STV[1000][100];  //list of all variables that are declared
int varnum = 0;       //number to keep track of how may variables were created
int goodinsert;       
int gooddeclare;
char myRO[10];        //string to hold a comparison operator
char mylabel[1000][20];    //array of labels that get created
int labelnum = 0;         //number to keep track of created labels new number
int currentlab = 0;       //number to keep track of what label should be used
char looplabel[20];       //string of loop label
int tempnum = 0;          //number to keep track of new temp number being created
char newtemp[10];         //string to create a new temp number to be saved

//function to move through tree to test what type of node it is and run approapriate function 
void checksem(struct treenode *sr)
{
   if(strcmp(sr->label, "vars") == 0 || strcmp(sr->label, "mvars") == 0)
   {
      goodinsert = insertvar(sr->treestring);   //insert variable to list if it has not been
      if(goodinsert == 1)
      {
         printf("Variable %s on line %d already declared\n", sr->treestring, sr->treelinenum);
         exit(0);
      }
      else
      {
          //variable has been declared
      }
      varnum += 1; 
   }
   if(strcmp(sr->label, "R") == 0 || strcmp(sr->label, "in") == 0 || strcmp(sr->label, "out") == 0 || strcmp(sr->label, "assign") == 0)
   {
      if(strcmp(sr->treetoken, "IDtk") == 0)
      {
         gooddeclare = verifyvar(sr->treestring);  //check if a variable being used has already been declared
         if(gooddeclare == 1)
         {
            printf("Variable %s on line %d not declared\n", sr->treestring, sr->treelinenum);
            exit(0);
         }
         else
         {
            //variable previously declared
         }
      }
   }
   if(sr->child1 != NULL)
   {
      checksem(sr->child1);  //move to child 1 if it exists
   }
   if(sr->child2 != NULL)
   {
      checksem(sr->child2);   //move to child 2 if is exists
   }
   if(sr->child3 != NULL)
   {
      checksem(sr->child3);   //move to child 3 if it exists
   }
   if(sr->child4 != NULL)
   {
      checksem(sr->child4);   //move to child 4 if it exists
   }
   return;   //move back up the tree once you are done with this node
}

//function to add variable to list if it exists
int insertvar(char variable[])
{
   int declared = 0, i;
   for(i = 0; i < 100; i++)
   {
      if(strcmp(STV[i], variable) == 0)  //check if this variable is already delared in this scope
      {
         declared += 1;
      }
   }
   if(declared == 0)   //if variable not already declared add it to the array
   {
      strcpy(STV[varnum], variable);  //add variable to STV
      return 0;  //pass
   }
   else
   {
      return 1;   //fail
   }
}

//checks if a variable has already been declared
int verifyvar(char variable[])
{
   int i, j;
      for(i = 0; i < varnum; i++)
      {
         if(strcmp(STV[i], variable) == 0)
         {
            return 0;   //has been declared
         }
      }
   return 1;   //has not been declared
}

//function to generate asm code 
void codegen(struct treenode *sr, FILE *f)
{
   int newvar, i;
   if(strcmp(sr->label, "program") == 0)
   {
      //no codegen needed
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
      fprintf(f, "\nSTOP\n");  
      for(i = 0; i < 100; i++)
      {
         if(strcmp(STV[i], "") != 0)
         {
            fprintf(f, "%s 0\n", STV[i]);   //print all variable set to zero at bottom of asm
         }
      }
   }
   if(strcmp(sr->label, "block") == 0)
   {
      //nocodegen needed
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
   }
   if(strcmp(sr->label, "vars") == 0)
   {
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
   }
   if(strcmp(sr->label, "mvars") == 0)
   {
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
   }
   if(strcmp(sr->label, "expr") == 0)
   {
      if(strcmp(sr->treestring, "+") == 0)
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
         sprintf(newtemp, "T%d", tempnum);
         tempnum++;
         newvar = insertvar(newtemp);
         fprintf(f, "STORE %s\n", newtemp);
         if(sr->child2 != NULL)
         {
            codegen(sr->child2, f);
         }
         fprintf(f, "ADD %s\n", newtemp);
      }
      else
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
      }
   }
   if(strcmp(sr->label, "M") == 0)
   {
      if(strcmp(sr->treestring, "-") == 0)
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
         sprintf(newtemp, "T%d", tempnum);
         tempnum++;
         varnum += 1;
         newvar = insertvar(newtemp);
         fprintf(f, "STORE %s\n", newtemp);
         if(sr->child2 != NULL)
         {
            codegen(sr->child2, f);
         }
         fprintf(f, "SUB %s\n", newtemp);
      }
      else
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
      }
   }
   if(strcmp(sr->label, "T") == 0)
   {
      if(strcmp(sr->treestring, "*") == 0 || strcmp(sr->treestring, "/") == 0)
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
         sprintf(newtemp, "T%d", tempnum);
         tempnum++;
         varnum += 1;
         newvar = insertvar(newtemp);
         fprintf(f, "STORE %s\n", newtemp);
         if(sr->child2 != NULL)
         {
            codegen(sr->child2, f);
         }
         if(strcmp(sr->treestring, "*") == 0)
         {
            fprintf(f, "MULT %s\n", newtemp);
         }
         if(strcmp(sr->treestring, "/") == 0)
         {
            fprintf(f, "DIV %s\n", newtemp);
         }
      }
      else
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
      }
   }
   if(strcmp(sr->label, "F") == 0)
   {
      if(strcmp(sr->treestring, "-") == 0)
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
         fprintf(f, "MULT -1\n");
      }
      else
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
      }
   }
   if(strcmp(sr->label, "R") == 0)
   {
      if(strcmp(sr->treetoken, "IDtk") == 0 || strcmp(sr->treetoken, "#tk") == 0)
      {
         fprintf(f, "LOAD %s\n", sr->treestring);
      }
      else
      {
         if(sr->child1 != NULL)
         {
            codegen(sr->child1, f);
         }
      }
   }
   if(strcmp(sr->label, "stats") == 0)
   {
      //no codegen needed
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
   }
   if(strcmp(sr->label, "mStat") == 0)
   {
      //no codegen needed
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
   }
   if(strcmp(sr->label, "stat") == 0)
   {
      //no codegen needed
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
   }
   if(strcmp(sr->label, "in") == 0)
   {
      fprintf(f, "READ %s\n", sr->treestring);
   }
   if(strcmp(sr->label, "out") == 0)
   {
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      sprintf(newtemp, "T%d", tempnum);
      tempnum++;
      varnum += 1;
      newvar = insertvar(newtemp);
      fprintf(f, "STORE %s\n", newtemp);
      fprintf(f, "WRITE %s\n", newtemp);
   }
   if(strcmp(sr->label, "iff") == 0)
   {
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
      if(sr->child3 != NULL)
      {
         codegen(sr->child3, f);
      }
      sprintf(newtemp, "T%d", tempnum);
      tempnum++;
      varnum += 1;
      newvar = insertvar(newtemp);
      fprintf(f, "STORE %s\n", newtemp);
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      fprintf(f, "SUB %s\n", newtemp);
      currentlab += 1;
      sprintf(mylabel[currentlab], "Label%d", labelnum);
      labelnum++;
      if(strcmp(myRO, ">=> ") == 0)
      {
         fprintf(f, "BRNEG %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "<=< ") == 0)
      {
         fprintf(f, "BRPOS %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "= ") == 0)
      {
         fprintf(f, "BRNEG %s\n", mylabel[currentlab]);
         fprintf(f, "BRPOS %s\n", mylabel);
      }
      if(strcmp(myRO, "> ") == 0)
      {
         fprintf(f, "BRZNEG %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "< ") == 0)
      {
         fprintf(f, "BRZPOS %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "=!= ") == 0)
      {
         fprintf(f, "BRZERO %s\n", mylabel[currentlab]);
      }
      if(sr->child4 != NULL)
      {
         codegen(sr->child4, f);
      }
      fprintf(f, "\n%s: NOOP\n", mylabel[currentlab]);
      currentlab -= 1;
   }
   if(strcmp(sr->label, "loop") == 0)
   {
      currentlab += 1;
      sprintf(mylabel[currentlab], "Label%d", labelnum);
      labelnum++;
      strcpy(looplabel, mylabel[currentlab]);
      fprintf(f, "\n%s: NOOP\n", looplabel);
      if(sr->child2 != NULL)
      {
         codegen(sr->child2, f);
      }
      if(sr->child3 != NULL)
      {
         codegen(sr->child3, f);
      }
      sprintf(newtemp, "T%d", tempnum);
      tempnum++;
      varnum += 1;
      newvar = insertvar(newtemp);
      fprintf(f, "STORE %s\n", newtemp);
      currentlab += 1;
      sprintf(mylabel[currentlab], "Label%d", labelnum);
      labelnum++;
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      fprintf(f, "SUB %s\n", newtemp);
      if(strcmp(myRO, ">=> ") == 0)
      {
         fprintf(f, "BRNEG %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "<=< ") == 0)
      {
         fprintf(f, "BRPOS %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "= ") == 0)
      {
         fprintf(f, "BRNEG %s\n", mylabel[currentlab]);
         fprintf(f, "BRPOS %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "> ") == 0)
      {
         fprintf(f, "BRZNEG %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "< ") == 0)
      {
         fprintf(f, "BRZPOS %s\n", mylabel[currentlab]);
      }
      if(strcmp(myRO, "=!= ") == 0)
      {
         fprintf(f, "BRZERO %s\n", mylabel[currentlab]);
      }
      if(sr->child4 != NULL)
      {
         codegen(sr->child4, f);
      }
      strcpy(looplabel, mylabel[currentlab]);
      fprintf(f, "BR %s\n", looplabel);
      fprintf(f, "\n%s: NOOP\n", mylabel[currentlab]);
      currentlab -= 2;
   }
   if(strcmp(sr->label, "assign") == 0)
   {
      if(sr->child1 != NULL)
      {
         codegen(sr->child1, f);
      }
      fprintf(f, "STORE %s\n", sr->treestring);
   }
   if(strcmp(sr->label, "RO") == 0)
   {
      strcpy(myRO, sr->treestring);  //retrieve relational operator   
   }
   return;   //move higher in the tree
}
       
