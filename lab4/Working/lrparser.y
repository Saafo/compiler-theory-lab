%{

#include <stdio.h>
#include "ast.h"
#include <string.h>
int yylex(void);
void yyerror(char *);
extern past astRoot;

%}

%union{
	int iValue;
	char sValue[50];
	past pAst;
};

%token <iValue> NUMBER
%token INT
%token IF
%token ELSE
%token <sValue> ID
%token <iValue> CMP

%type  <pAst>	declaration_statement statement_list selection_statement
		statement expr assign_statement cmp_expr add_expr mul_expr primary_expr

%%

statement_list: statement {$$ = newList(NULL, $1); astRoot = $$; }
	      | statement_list statement { $$ = newList($1, $2); }

statement: selection_statement
	 | declaration_statement
	 | assign_statement

selection_statement: IF '(' expr ')' statement  {$$ = newStmt("sele",$3,$5,NULL);}
		   | IF '(' expr ')' statement ELSE statement {$$ = newStmt("sele",$3,$5,$7);}

declaration_statement:INT ID ';' {$$ = newStmt("decl",newID($2),NULL,NULL);}
		     |INT ID '=' add_expr ';' {$$ = newStmt("decl",newID($2),$4,NULL);}

expr: cmp_expr{}

assign_statement: ID '=' cmp_expr ';' {$$ = newStmt("assign",newID($1),$3,NULL);}

cmp_expr: add_expr {}
	| cmp_expr CMP add_expr {$$ = newExpr($2,"cmp",$1,$3,NULL);}

add_expr: mul_expr
        | add_expr '+' mul_expr {$$ = newExpr('+',"add",$1,$3,NULL);}
        | add_expr '-' mul_expr {$$ = newExpr('-',"sub",$1,$3,NULL);}

mul_expr: primary_expr
        | mul_expr '*' primary_expr {$$ = newExpr('*',"mul",$1,$3,NULL);}
        | mul_expr '/' primary_expr {$$ = newExpr('/',"mul",$1,$3,NULL);}
        | mul_expr '%' primary_expr {$$ = newExpr('%',"mul",$1,$3,NULL);}
        | '-' primary_expr {$$ = newExpr('N',"mul",$2,NULL,NULL);}

primary_expr: ID {$$ = newID($1);}
            | NUMBER {$$ = newNum($1);}

%%

