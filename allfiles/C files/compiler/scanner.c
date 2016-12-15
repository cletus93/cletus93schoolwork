#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"

int Table[30][200];
int i, j;

//build table for token traversal
void createTable()
{
   for(i = 0; i < 30; i++)
   {
      for(j = 0; j < 200; j++)
      {
         Table[i][j] = -1;       //populate array with errors
      }
   }
   for(i = 65; i < 91; i++)
   {
      Table[0][i] = 1;          //start with letter goes to state 1
   }
   for(i = 97; i < 123; i++)
   {
      Table[0][i] = 1;          //start with letter goes to state 1
   }
   for(i = 48; i < 58; i++)
   {
      Table[0][i] = 2;          //start with number goes to state 2
   }
   Table[0][32] = 0;            //ws goes to state 0
   Table[0][26] = 1026;         //EOFtk reached 
   Table[0][61] = 3;            //= goes to state 3
   Table[0][60] = 6;            //< goes to state 6
   Table[0][62] = 9;            //> goes to state 9
   Table[0][33] = 12;           //! goes to state 12
   Table[0][43] = 13;           //+ goes to state 13
   Table[0][45] = 14;           //- goes to state 14
   Table[0][42] = 15;           //* goes to state 15
   Table[0][47] = 16;           /// goes to state 16
   Table[0][38] = 17;           //& goes to state 17
   Table[0][37] = 18;           //% goes to state 18
   Table[0][46] = 19;           //. goes to state 19
   Table[0][40] = 20;           //( goes to state 20
   Table[0][41] = 21;           //) goes to state 21
   Table[0][59] = 22;           //; goes to state 22
   Table[0][123] = 23;          //{ goes to state 23
   Table[0][125] = 24;          //} goes to state 24
   Table[0][91] = 25;           //[ goes to state 25
   Table[0][93] = 26;           //] goes to state 26
   Table[0][44] = 27;           //, goes to state 27
   Table[0][58] = 29;             //: goes to state 29
   for(i = 65; i < 91; i++)
   {
      Table[1][i] = 1;          //all letters loop in state 1
   }
   for(i = 97; i < 123; i++)
   {
      Table[1][i] = 1;          //all letters loop in state 1
   }
   for(i = 48; i < 58; i++)
   {
      Table[1][i] = 1;          //all numbers loop in state 1
   }
   Table[1][32] = 1000;         //IDtk reached
   for(i = 48; i < 58; i++)
   {
      Table[2][i] = 2;          //all numbers loop in state 2
   }
   Table[2][32] = 1001;         //#tk reached
   Table[3][32] = 1002;         //=tk reached           
   Table[3][61] = 28;           //= goes to state 28
   Table[3][33] = 4;            //! goes to state 4
   Table[4][61] = 5;            //= goes to state 5
   Table[5][32] = 1003;         //=!=tk reached
   Table[6][32] = 1004;         //<tk reached
   Table[6][61] = 7;            //= goes to state 7
   Table[7][60] = 8;            //< goes to state 8
   Table[8][32] = 1005;         //<=<tk reached
   Table[9][32] = 1006;         //>tk reached
   Table[9][61] = 10;           //= goes to state 10
   Table[10][62] = 11;          //> goes to state 11
   Table[11][32] = 1007;        //>=>tk reached
   Table[12][32] = 1008;        //!tk reached
   Table[13][32] = 1009;        //+tk reached
   Table[14][32] = 1010;        //-tk reached
   Table[15][32] = 1011;        //*tk reached
   Table[16][32] = 1012;        ///tk reached
   Table[17][32] = 1013;        //&tk reached
   Table[18][32] = 1014;        //%tk reached
   Table[19][32] = 1015;        //.tk reached
   Table[20][32] = 1016;        //(tk reached
   Table[21][32] = 1017;        //)tk reached
   Table[22][32] = 1018;        //;tk reached
   Table[23][32] = 1019;        //{tk reached
   Table[24][32] = 1020;        //}tk reached
   Table[25][32] = 1021;        //[tk reached
   Table[26][32] = 1022;        //]tk reached 
   Table[27][32] = 1023;        //,tk reached
   Table[28][32] = 1024;        //==tk reached
   Table[29][32] = 1025;        //:tk reached
}

int checkkeywords(char * checkptr)
{
   //establish all keywords to look for
   const char *keywords[14] = {"Begin", "End", "Start", "Stop", "Iff", "Loop", "Void", "Var", "Int", "Call", "Return", "Scan", "Print", "Program"};
   char stringtocheck[10];
   int cmpnum = 0;
 
   sprintf(stringtocheck, "%s", checkptr);
   for(i = 0; i < 14; i++)
   {
      if(strcmp(keywords[i], stringtocheck) == 0) //compare string to all keywords
      {
         return 1;  //if one matches return 1 
      }
   }
   return 0;   //else return 0
}

//moves through table based on states and current character in the string
struct token TokenDriver(char * inputstring, int linecount)
{
   int charcount = 0;
   int state = 0;
   int nextstate;
   int nextChar;
   char string[10];
   struct token gentoken;
    
   sprintf(string, "%s ", inputstring); 
   nextChar = string[charcount];      //get first character

   createTable();   //build table

   while(state < 1000)   //move through table until the state is less that 0 or greater than 999
   {
      nextstate = Table[state][nextChar];  //assign next state from table
      if(nextstate == -1)  //detect invalid entry
      {
         printf("Error invalid entry\n");
         exit(0);
      }
      if(nextstate > 999)  //detect final state and figure out which one it is
      {
         if(nextstate == 1000)  
         {
            if(checkkeywords(inputstring) == 1)   //check if string is a keyword
            {
              gentoken.mytoken = "KEYWORDtk";
              sprintf(gentoken.mystring, "%s", string);
              gentoken.linenum = linecount;
              return gentoken;
            }
            else
            {
              gentoken.mytoken = "IDtk";
              sprintf(gentoken.mystring, "%s", string);
              gentoken.linenum = linecount;
              return gentoken;
            }
         }
         else if(nextstate == 1001)
         {
            gentoken.mytoken = "#tk";
            sprintf(gentoken.mystring, "%s", string); 
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1002)
         {
            gentoken.mytoken = "=tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1003)
         {
            gentoken.mytoken = "=!=tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1004)
         {
            gentoken.mytoken = "<tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1005)
         {
            gentoken.mytoken = "<=<tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1006)
         {
            gentoken.mytoken = ">tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1007)
         {
            gentoken.mytoken = ">=>tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1008)
         {
            gentoken.mytoken = "!tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1009)
         {
            gentoken.mytoken = "+tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1010)
         {
            gentoken.mytoken = "-tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1011)
         {
            gentoken.mytoken = "*tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1012)
         {
            gentoken.mytoken = "/tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1013)
         {
            gentoken.mytoken = "&tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1014)
         {
            gentoken.mytoken = "%tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1015)
         {
            gentoken.mytoken = ".tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1016)
         {
            gentoken.mytoken = "(tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1017)
         {
            gentoken.mytoken = ")tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1018)
         {
            gentoken.mytoken = ";tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1019)
         {
            gentoken.mytoken = "{tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1020)
         {
            gentoken.mytoken = "}tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1021)
         {
            gentoken.mytoken = "[tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1022)
         {
            gentoken.mytoken = "]tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1023)
         {
            gentoken.mytoken = ",tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1024)
         {
            gentoken.mytoken = "==tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if(nextstate == 1025)
         {
            gentoken.mytoken = ":tk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
         else if (nextstate == 1026)
         {
            gentoken.mytoken = "EOFtk";
            sprintf(gentoken.mystring, "%s", string);
            gentoken.linenum = linecount;
            return gentoken;
         }
      state = nextstate;
      }
      else   //if not in final state or error
      {
         state = nextstate;  //move to next state
         charcount++;        //move to next character in the string
         nextChar = string[charcount];
      }
   }
}
