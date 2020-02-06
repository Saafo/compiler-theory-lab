#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "genllvm.h"


extern int temVarNum;
past astRoot;
extern int yyparse();
extern FILE *yyin;

void yyerror(char *s)
{
    printf("%s\n", s);
}

int main(int argc, char *argv[])
{
    extern int yyparse(void);
    extern FILE *yyin;


    if (argc < 2)
        yyin = fopen("./expr.txt", "r");
    else
        yyin = fopen(argv[1], "r");

    if (yyparse())
    {
        printf("An Error Occurred.\n");
        return -1;
    }
    fclose(yyin);

    past stmt = astRoot->next;
    int count = 1;
    while( stmt != NULL)
    {
        printf("\n\n  ==========Statement %d:===========\n", count++);
        showAst(stmt, 1);
        printf("\n");
        genStmt(stmt);
        stmt = stmt->next;
    }
    return 0;
}
