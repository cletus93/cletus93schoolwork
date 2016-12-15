#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include "compilertree.h"
#include "treePrint.h"
#include "scanner.h"
#include "parser.h"
#include "buildTree2.h"

typedef enum tokenID
{
	IDtk, 
	NUMtk, 
	KEYWORDtk, 
	EQUtk, 
	LESStk, 
	GRTRtk, 
	NOTEQtk, 
	LSEQtk, 
	GRTEQtk,
	EQUIVtk,
	COLtk,
	PLUStk,
	MULTtk,
	DIVtk,
	AMPtk,
	MODtk,
	PRDtk,
	LPARtk,
	RPARtk,
	SEMCOLtk,
	CRVLBRtk,
	CRVRBRtk,
	SQLBRtk,
	SQRBRtk,
	COMtk
}tokenID;

typedef struct token
{
   char *mytoken;
   char mystring[15];
   int linenum;
}token;

extern const char *keywords[14];

#endif
