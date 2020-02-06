%{
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include"lrparser.tab.h"


%}


digit           [0-9]
letter          [a-zA-Z]

ID              ({letter}|"_"|"$")({letter}|"_"|"$"|{digit})*
DELIMITER       "{"|"}"|","|";"|"("|")"|"["|"]"
OPERATOR        "+"|"-"|"*"|"/"|"%"
CMP             ">"|"<"
ASSIGN          "="
NUMBER          {digit}+


%%
"int"           { return INT;  }
"if"            { return IF; }
"else"          { return ELSE; }

{NUMBER}        { yylval.iValue = atoi(yytext); return NUMBER; }
{ID}            { strcpy(yylval.sValue,yytext); return ID; }

{DELIMITER}      { return yytext[0];}

{CMP}            { yylval.iValue = yytext[0];return CMP;}

{ASSIGN}         { return yytext[0];}

{OPERATOR}       { return yytext[0];}



[ \n\t] { /* ignore whitespace */ }
. 	{ printf("Mystery character %c\n", *yytext); }


%%


int yyerror(char *s);

int yywrap()
{
    return 1;
}
