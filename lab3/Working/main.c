#include<stdio.h>
#include<stdlib.h>
#include "ast.h"

extern FILE *yyin;
extern int yyparse(void);

int main(int argc,char **argv)
{
    if(argc<2){
        printf("ERROR:input file name is needed.\n");
        exit(0);
    }
    yyin = fopen(argv[1],"r");
    if(yyin == NULL){
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    yyparse();
    showAst(astHead,0);

    return 0;
}