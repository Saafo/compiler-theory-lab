
#ifndef GENLLVM_H
#define GENLLVM_H

#include "ast.h"


#define true 1
#define false 0

void genExpr(past node);
void genPrimaryExpr(past node, char* operand);
void genDecl(past node);
void genSele(past node);
void genAssgin(past node);
void genStmt(past node);
#endif
