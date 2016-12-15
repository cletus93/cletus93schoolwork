#ifndef SEMANTICS_H
#define SEMANTICS_H
#include <stdio.h>
#include "token.h"
#include "compilertree.h"

void checksem(struct treenode *sr);
int insertvar(char);
int verifyvar(char);
void codegen(struct treenode *sr, FILE*);

#endif
