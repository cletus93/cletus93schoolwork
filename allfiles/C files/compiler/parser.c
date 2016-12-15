//functions to be used in checking semantics of a program, keeps track of variables and syntax
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.h"
#include "compilertree.h"
#include "treePrint.h"
#include "buildTree2.h"

struct token storetoken[1000]; 
int tokennum;
char mytoken[10];
char mystring[10];

//function seperates all words in a file by white space and operates on them one at a time
char * scannerparse(int type, FILE * file, int size, FILE * f)
{
   char stringtoparse[100];
   char parsedstring[100];
   char *strptr =  malloc(sizeof(int) * size); 
   int linecount = 0, arrayspot = 0;
   struct token gentoken;

   if(type == 1)     //checks if a file is passed
   {
      while(fgets ( stringtoparse, sizeof(stringtoparse), file ) != NULL)  //parse string
      {
         linecount++;
         strptr = strtok (stringtoparse, " \n");
         while(strptr != NULL)   //do operations if strptr has a string
         {
            strcpy(parsedstring, strptr);  //copy string so you can look at first element
            if(parsedstring[0] == '@')  //if first element is @ then it is a comment and needs to be excluded
            {
               strptr = strtok(NULL, " \n");  //clear string ptr to move on
            }
            else  //if string is not a comment pass it to the scanner to find out the token
            {
               storetoken[arrayspot] = TokenDriver(strptr, linecount);   //call to find token
               arrayspot++;
               strptr = strtok(NULL, " \n");
            }
         }
      }
      storetoken[arrayspot].mytoken = "EOFtk";
      storetoken[tokennum].linenum = linecount;
   }
   else
   {
      while(fgets ( stringtoparse, sizeof(stringtoparse), file ) != NULL)  //parse file
      {
         strptr = strtok (stringtoparse, " \n");
         while(strptr != NULL)   //check if strptr has a string in it
         {
            strcpy(parsedstring, strptr);  //copy to look at first element
            if(parsedstring[0] == '@')   //if comment, skip word
            {
               strptr = strtok(NULL, " \n");
            }
            else    //pass word to get token
            {
               storetoken[arrayspot] = TokenDriver(strptr, linecount);   //call to find token
               arrayspot++;
               strptr = strtok(NULL, " \n");
            }
         }
      }
      storetoken[arrayspot].mytoken = "EOFtk ";
      storetoken[tokennum].linenum = linecount;
   }
   int depth = 0;
   struct treenode* root;
   root = buildprogramtree(depth);
   preorder ( root );
   printpreordertree(root);
   checksem(root);
   codegen(root, f);
   return strptr;
}

struct treenode* buildprogramtree(int depth)
{
   struct treenode* root;
   tokennum = 0;
   root->level = depth;
   root = program(depth);
   if(strcmp(storetoken[tokennum].mytoken, "EOFtk "))
   {
      printf("parse was ok\n");
   }
   else 
   {
      printf("Error in syntax, no EOFtk\n"); // error message, exit, no recovery
      exit(0);
   }
   return root;
}

struct treenode* program(int depth)
{
   struct treenode* p;
   insert("program", &p, depth, storetoken[tokennum].linenum); 
   p->child1 = vars(depth);
   p->child2 = block(depth);
   return p;
}

struct treenode* block(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("block", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Begin ") == 0)
   {
     tokennum++;
     p->child1 = vars(depth);
     p->child2 = stats(depth);
     if (strcmp(storetoken[tokennum].mystring, "End ") == 0)
     {
        tokennum++;
     }
     else
     {
        printf("Error in syntax, missing End\n"); // error message, exit, no recovery
        exit(0);
     }
     return p;
   }
   else
   {
      printf("Error in syntax, missing Begin\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* vars(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("vars", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Var ") == 0)
   {
      tokennum++;
      if (strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
      {
         strcpy(p->treetoken, storetoken[tokennum].mytoken);
         strcpy(p->treestring, storetoken[tokennum].mystring);
         p->level = depth;
         tokennum++;
         p->child1 = mvars(depth);
      }
      else
      {
         printf("Error in syntax, missing IDtk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      return NULL;
   }
}

struct treenode* mvars(int depth)
{
   struct treenode* p;
   depth += 1;
   if(strcmp(storetoken[tokennum].mytoken, ":tk") == 0)
   {
      tokennum++;
      if(strcmp(storetoken[tokennum].mytoken, ":tk") == 0)
      {
         tokennum++;
         if(strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
         {
            insert("mvars", &p, depth, storetoken[tokennum].linenum);
            strcpy(p->treetoken, storetoken[tokennum].mytoken);
            strcpy(p->treestring, storetoken[tokennum].mystring);
            tokennum++;
            p->child1 = mvars(depth);
         }
         else
         {
            printf("Error in syntax, missing IDtk\n"); // error message, exit, no recovery
            exit(0);
         }

      }
      else
      {
         printf("Error in syntax, missing :\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      return NULL;
   }

}

struct treenode* expr(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("expr", &p, depth, storetoken[tokennum].linenum);  //call to add variable to variable array
   p->child1 = M(depth);
   if(strcmp(storetoken[tokennum].mytoken, "+tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      p->child2 = expr(depth);
   }
   return p;
}

struct treenode* M(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("M", &p, depth, storetoken[tokennum].linenum);
   p->child1 = T(depth);
   if(strcmp(storetoken[tokennum].mytoken, "-tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      p->child1 = M(depth);
   }
   return p;
}
 
struct treenode* T(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("T", &p, depth, storetoken[tokennum].linenum);
   p->child1 = F(depth);
   if(strcmp(storetoken[tokennum].mytoken, "*tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      p->child2 = T(depth);
   }
   else if(strcmp(storetoken[tokennum].mytoken, "/tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      p->child2 = T(depth);
   }
   return p;
}

struct treenode* F(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("F", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mytoken, "-tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      p->child1 = F(depth);
      return p;
   }
   else
   {
      p->child1 = R(depth);
      return p;
   }
}

struct treenode* R(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("R", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mytoken, "[tk") == 0)
   {
      tokennum++;
      expr(depth);
      if(strcmp(storetoken[tokennum].mytoken, "]tk") == 0)
      {
         tokennum++;
      }
      else
      {
         printf("Error in syntax, missing ]tk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "#tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else
   {
      printf("Error in syntax, missing IDtk or #tk\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* stats(int depth)
{ 
   struct treenode* p;
   depth += 1;
   insert("stats", &p, depth, storetoken[tokennum].linenum);
   p->child1 = stat(depth);
   p->child2 = mStat(depth);
   return p;
}

struct treenode* mStat(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("mStat", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Scan ") == 0 || strcmp(storetoken[tokennum].mystring, "Print ") == 0 || strcmp(storetoken[tokennum].mystring, "Begin ") == 0 || strcmp(storetoken[tokennum].mytoken, "[tk") == 0 || strcmp(storetoken[tokennum].mystring, "Loop ") == 0 || strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
   {
      p->child1 = stat(depth);
      p->child2 = mStat(depth);
   }
   else
   {
      return NULL;
   }
   return p;
}

struct treenode* stat(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("stat", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Scan ") == 0)
   {
      p->child1 = in(depth);
      return p;
   }
   else if(strcmp(storetoken[tokennum].mystring, "Print ") == 0)
   {
      p->child1 = out(depth);
      return p;
   }
   else if(strcmp(storetoken[tokennum].mystring, "Begin ") == 0)
   {
      p->child1 = block(depth);
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "[tk") == 0)
   {
      p->child1 = iff(depth);
      return p;
   }
   else if(strcmp(storetoken[tokennum].mystring, "Loop ") == 0)
   {
      p->child1 = loop(depth);
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
   {
      p->child1 = assign(depth);
      return p;
   }
   else
   {
      printf("Error in syntax, missing tk\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* in(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("in", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Scan ") == 0)
   {
      tokennum++;
      if(strcmp(storetoken[tokennum].mytoken, ":tk") == 0)
      {
         tokennum++;
         if(strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
         {
            strcpy(p->treetoken, storetoken[tokennum].mytoken);
            strcpy(p->treestring, storetoken[tokennum].mystring);
            tokennum++;
            if(strcmp(storetoken[tokennum].mytoken, ".tk") == 0)
            {
               tokennum++;
            }
            else
            {
                printf("Error in syntax, missing .tk\n"); // error message, exit, no recovery
                exit(0);
            }
         }
         else
         {
            printf("Error in syntax, missing IDtk\n"); // error message, exit, no recovery
            exit(0);
         }
      }
      else
      {
         printf("Error in syntax, missing :tk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      printf("Error in syntax, missing Scan\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* out(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("out", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Print ") == 0)
   {
      tokennum++;
      if(strcmp(storetoken[tokennum].mytoken, "[tk") == 0)
      {
         tokennum++;
         p->child1 = expr(depth);
         if(strcmp(storetoken[tokennum].mytoken, "]tk") == 0)
         {
            tokennum++;
            if(strcmp(storetoken[tokennum].mytoken, ".tk") == 0)
            {
               tokennum++;
            }
            else
            {
               printf("Error in syntax, missing .tk\n"); // error message, exit, no recovery
               exit(0);
            }
         }
         else
         {
            printf("Error in syntax, ]tk\n"); // error message, exit, no recovery
            exit(0);
         }
      }
      else
      {
         printf("Error in syntax, [tk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      printf("Error in syntax, missing print\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* iff(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("iff", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mytoken, "[tk") == 0)
   {
      tokennum++;
      p->child1 = expr(depth);
      p->child2 = RO(depth);
      p->child3 = expr(depth);
      if(strcmp(storetoken[tokennum].mytoken, "]tk") == 0)
      {
         tokennum++;
         if(strcmp(storetoken[tokennum].mystring, "Iff ") == 0)
         {
           tokennum++;
           p->child4 = block(depth);
         }
         else
         {
            printf("Error in syntax, missing Iff\n"); // error message, exit, no recovery
            exit(0);
         }
      }
      else
      {
         printf("Error in syntax, missing ]tk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      printf("Error in syntax, missing [tk\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* loop(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("loop", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mystring, "Loop ") == 0)
   {
      tokennum++;
      if(strcmp(storetoken[tokennum].mytoken, "[tk") == 0)
      {
         tokennum++;
         p->child1 = expr(depth);
         p->child2 = RO(depth);
         p->child3 = expr(depth);
         if(strcmp(storetoken[tokennum].mytoken, "]tk") == 0)
         {
            tokennum++;
            p->child4 = block(depth);
         }
         else
         {
            printf("Error in syntax, missing ]tk\n"); // error message, exit, no recovery
            exit(0);
         }
      }
      else
      {
         printf("Error in syntax, missing [tk\n"); // error message, exit, no recovery
         exit(0);
      }
      return p;
   }
   else
   {
      printf("Error in syntax, missing Loop\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* assign(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("assign", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mytoken, "IDtk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      if(strcmp(storetoken[tokennum].mytoken, "==tk") == 0)
      {
         strcpy(p->treetoken2, storetoken[tokennum].mytoken);
         strcpy(p->treestring2, storetoken[tokennum].mystring);
         tokennum++;
         p->child1 = expr(depth);
         if(strcmp(storetoken[tokennum].mytoken, ".tk") == 0)
         {
            tokennum++;
         }
         else
         {
            printf("Error in syntax, missing .tk\n"); // error message, exit, no recovery
            exit(0);
         }
      }
      else
      {
         printf("Error in syntax, missing ==tk\n"); // error message, exit, no recovery
         exit(0);
      }
   return p;
   }
   else
   {
      printf("Error in syntax, missing IDtk\n"); // error message, exit, no recovery
      exit(0);
   }
}

struct treenode* RO(int depth)
{
   struct treenode* p;
   depth += 1;
   insert("RO", &p, depth, storetoken[tokennum].linenum);
   if(strcmp(storetoken[tokennum].mytoken, ">=>tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "<=<tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "=tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, ">tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "<tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else if(strcmp(storetoken[tokennum].mytoken, "=!=tk") == 0)
   {
      strcpy(p->treetoken, storetoken[tokennum].mytoken);
      strcpy(p->treestring, storetoken[tokennum].mystring);
      tokennum++;
      return p;
   }
   else
   {
      printf("Error in syntax, missing operator tk\n"); // error message, exit, no recovery
      exit(0);
   }
}

