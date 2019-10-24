%{
#define STRING_MAX 100
enum yytokentype {
	NUMBER = 258,ID,ASSIGN,STRING,CMP,SCAN,PRINT,RETURN,WHILE,IF,ELSE,INT,STR,VOID
};

int yylval;
char* yypstr;

%}


%%

\/\/.*\n //expireComments
"+" | "-" | "*" | "/" | \% | "=" | ";" | "{" | "}" | "[" | "]" | "(" | ")" | "," | "q" { return yytext[0]; }
\".*\" { yypstr = (char*)malloc(sizeof(yytext)); strcpy(yypstr,yytext); return STRING;} //String
[0-9]+ { yylval = atoi(yytext); return NUMBER; } //Number
\+= | -= | \*= | \/= { return ASSIGN; }
== { return CMP; }
 //接下来是keywords
[a-zA-Z_][a-zA-Z_0-9]* { yypstr = (char*)malloc(sizeof(yytext)); strcpy(yypstr,yytext); return ID; }
scan {return SCAN; }
print { return PRINT; }
return { return RETURN; }
while { return WHILE; }
if { return IF; }
else { return ELSE; }
int {return INT; }
str { return STR; }
void { return VOID; }
 //\n	{ return EOL; }
[ \t\r\n]+ { /* ignore whitespace */ }
. 	{ printf("Mystery character %c\n", *yytext); }

%%


int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}
