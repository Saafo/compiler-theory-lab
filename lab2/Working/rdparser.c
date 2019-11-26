/*
============================================================================
Name        : rdparser.c
Author      : @saafo
Version     : v0.1 Pre Alpha
Copyright   : Copyright (c) 2019 SaffordHuang All rights reserved.
Description : A Parser
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX 100
enum yytokentype {
    NUMBER = 258,ID,ASSIGN,STRING,CMP,SCAN,PRINT,RETURN,WHILE,IF,ELSE,INT,STR,VOID
};

extern int yylex();
extern int yylval;
extern char* yypstr;//记得free!!!
extern char* yytext;
extern FILE* yyin;


int tok;

void advance()
{
    tok = yylex();
    printf("tok: %s\n", yytext);
}

//--------------------------
// return value:
// 0:failed
// 1:success
// parameter(allow print error):
// 0:false
// 1:true(forced)
// recursiving:
// 0:flase
// 1:true(being recursving)
//--------------------------



typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
    union{
        int ivalue;
        char* svalue;
    };
    char* nodeType;
    past a;
    past b;
    past c;
    past d;
    past e;
    past f;
    past g;
    past h;
};

past newAstNode();
void freeNode(past);
past newNumber(int);
past newOperator(char*);
past newId(char*);
past newSymbol(char*);
past newString(char*);
past newKeyword(char*);
past id_list(int);
past expr_list(int);
past primary_expr(int);
past mul_expr(int);
past add_expr(int);
past cmp_expr(int);
past expr(int);
past expression_statement(int);
past statement_list(int);
past statement(int);
past type(int);
past parameter(int);
past parameter_list(int);
past declarator(int);
past initializer(int);
past intstr_list(int);
past declarator_list(int);
past decl_or_stmt(int);
past external_declaration(int);
past program(int);

past newAstNode()
{
    past node = malloc(sizeof(ast));
    if(!node)
    {
        printf("run out of memory.\n");
        exit(-2);
    }
    memset(node, NULL, sizeof(ast));//origin it's '0', but I change it to NULL
    return node;
}

void freeNode(past p)
{
	if(p == NULL)
		return;
    if(p->a != NULL)
        freeNode(p->a);
    if(p->b != NULL)
        freeNode(p->b);
    if(p->c != NULL)
        freeNode(p->c);
    if(p->d != NULL)
        freeNode(p->d);
    if(p->e != NULL)
        freeNode(p->e);
    if(p->f != NULL)
        freeNode(p->f);
    if(p->g != NULL)
        freeNode(p->g);
    if(p->h != NULL)
        freeNode(p->h);
	if(p->nodeType != NULL)
    	free(p);
	p = NULL;
}


past newNumber(int value)
{
    past var = newAstNode();
    var->ivalue = value;
    var->nodeType = "NUMBER";
    return var;
}

past newOperator(char* oper)//, past a, past b)
{
    past var = newAstNode();
    var->svalue = oper;
    var->nodeType = "OPERATOR";
    // var->a = a;
    // var->b = b;
    return var;
}

past newId(char* id)
{
    past var = newAstNode();
    var->svalue = id;
    var->nodeType = "ID";
    return var;
}

past newSymbol(char* symbol)//symbol: ()[]{}=;,(+=)(-=)(*=)(/=)
{
    past var = newAstNode();
    var->svalue = symbol;
    if(!strcmp(symbol,"(") || !strcmp(symbol,")")
     || !strcmp(symbol,"[") || !strcmp(symbol,"]"))
        var->nodeType = "Bracket";
    else if(!strcmp(symbol,"{") || !strcmp(symbol,"}"))
        var->nodeType = "Brace";
    else if(!strcmp(symbol,"=") || !strcmp(symbol,"+=")
     || !strcmp(symbol,"-=") || !strcmp(symbol,"*=")
     || !strcmp(symbol,"/="))
        var->nodeType = "ASSIGN";
    else if(!strcmp(symbol,";"))
        var->nodeType = "Semi";
    else if(!strcmp(symbol,","))
        var->nodeType = "Comma";
    return var;
}

past newString(char* string)
{
    past var = newAstNode();
    var->svalue = string;
    var->nodeType = "STRING";
    return var;
}

past newKeyword(char* keyword)
{
    past var = newAstNode();
    var->svalue = keyword;
    var->nodeType = "KEYWORD";
    return var;
}

//-------END___OF___NEW----------

past id_list(int t)
{
	past a = newAstNode();
	if(tok == ID)
	{
		a = newId(yypstr);
		past p = newAstNode();
		past pp = newAstNode();
		past b = newAstNode();
		past c = newAstNode();
		advance();
		while(tok == ',')
		{
			b = newSymbol(",");
			advance();
			if(tok != ID)
			{
				if(t == 1)
				{
					printf("Expected an ID.\n");
					exit(-1);
				}
				else
					return NULL;
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "id_list";
            p->b = pp;
			advance();
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "parameter-list";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past expr_list(int t)
{
	past a = newAstNode();
	if((a = expr(t)))
	{
		past p = newAstNode();
		past pp = newAstNode();
		past b = newAstNode();
		past c = newAstNode();
		p->a = a;
		while(tok == ',')
		{
			b = newSymbol(",");
			advance();
			if(!(c = expr(t)))
			{
				if(t == 1)
				{
					printf("Expected an expr.\n");
					exit(-1);
				}
				else
					return NULL;
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "expr_list";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "parameter-list";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past primary_expr(int t)
{
	past p = newAstNode();
	past a = newAstNode();
	past b = newAstNode();
	past c = newAstNode();
	past d = newAstNode();
	past e = newAstNode();
	past f = newAstNode();
	if(tok == ID)
	{
		a = newId(yypstr);
		advance();
		if (tok == '(')
		{
			b = newSymbol("(");
			advance();
			if((c = expr_list(t)))
			{
				if(tok == ')')
				{
					d = newSymbol(")");
					advance();
					p->a = a;
					p->b = b;
					p->c = c;
					p->d = d;
					p->nodeType = "Non-Terminal";
					p->svalue = "expr-list-primary-expr";
					return p;
				}
				else if(t == 1)
				{
					printf("Expected a ')'.\n");
					exit(-1);
				}
				else
					return NULL;
			}
			else if (tok == ')')
			{
				c = newSymbol(")");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "empty-bracket-primary-expr";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected ')' or expr_list.\n");
			}
			else
				return NULL;
		}
		else if (tok == ASSIGN)
		{
			b = newOperator(yypstr);
			advance();
			if((c = expr(t)))
			{
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "dual-assign-primary-expr";
				return p;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return NULL;
		}
		else if (tok == '=')
		{
			b = newSymbol("=");
			advance();
			if((c = expr(t)))
			{
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "assign-primary-expr";
				return p;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return NULL;
		}
		else if (tok == '[')
		{
			b = newSymbol("[");
			advance();
			if((c = expr(t)))
			{
				if(tok == ']')
				{
					d = newSymbol("]");
					advance();
					if(tok == '=')
					{
						e = newSymbol("=");
						if((f = expr(t)))
						{
							p->a = a;
							p->b = b;
							p->c = c;
							p->d = d;
							p->e = e;
							p->f = f;
							p->nodeType = "Non-Terminal";
							p->svalue = "array-assign-primary-expr";
							return p;
						}
						else if (t == 1)
						{
							printf("Expected expr.\n");
							exit(-1);
						}
						else
							return NULL;
					}
					else
					{
						p->a = a;
						p->b = b;
						p->c = c;
						p->d = d;
						p->nodeType = "Non-Terminal";
						p->svalue = "array-primary-expr";
						return p;
					}
				}
				else if (t == 1)
				{
					printf("Expected a ']'.\n");
					exit(-1);
				}
				else
					return NULL;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return NULL;
		}
		else
		{
			p->a = a;
			p->nodeType = "Non-Terminal";
			p->svalue = "id-primary-expr";
			return p;
		}
	}	
	else if (tok == NUMBER)
	{
		a = newNumber(yylval);
		advance();
		p->a = a;
		p->nodeType = "Non-Terminal";
		p->svalue = "number-primary-expr";
		return p;
	}
	else if (tok == STRING)
	{
		a = newString(yypstr);
		advance();
		p->a = a;
		p->nodeType = "Non-Terminal";
		p->svalue = "string-primary-expr";
		return p;
	}
	else if (tok == '(')
	{
		a = newSymbol("(");
		advance();
		if((b = expr(t)))
		{
			if(tok == ')')
			{
				c = newSymbol(")");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "expr-in-bracket-primary-expr";
				return p;
			}
			else if (t == 1)
			{
				printf("Expected a ')'.\n");
				exit(-1);
			}
			else
				return NULL;
		}
		else if (t == 1)
		{
			printf("Expected expr.\n");
			exit(-1);
		}
		else
			return NULL;
	}
	else
		return NULL;
}

past mul_expr(int t)
{
	past a = newAstNode();
	past p = newAstNode();
	past pp = newAstNode();
	if(tok == '-')
	{
		a = newOperator("-");
		p->a = a;
		advance();
	}
	past b = newAstNode();
	if((b = primary_expr(t)))
	{
		p->b = b;
		while(tok == '*' || tok == '/' || tok == '%')
		{
			past c = newAstNode();
			past d = newAstNode();
			past e = newAstNode();
			c = (tok == '*')?newOperator("*")
				:((tok == '/')?newOperator("/"):newOperator("%"));
			advance();
			if(tok == '-')
			{
				d = newOperator("-");
				advance();
			}
			if(!(e = primary_expr(t)))
			{
				if(t == 1)
				{
					printf("Expected primary_expr.\n");
					exit(-1);
				}
				else
					return NULL;
			}
            pp->b = p->c;
            pp->c = c;
            pp->d = d;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "mul_expr";
            p->c = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "mul_expr";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past add_expr(int t)
{
	past a = newAstNode();
	if((a = mul_expr(t)))
	{
		past p = newAstNode();
		past pp = newAstNode();
		p->a = a;
		while (tok == '+' || tok == '-')
		{
			past b = newAstNode();
			past c = newAstNode();
			b = (tok == '+')?newOperator("+"):newOperator("-");
			advance();
			if(!(c = mul_expr(t)))
			{
				if(t == 1)
				{
					printf("Expected mul_expr.\n");
					exit(-1);
				}
				else
                {
                    freeNode(p);
                    freeNode(pp);
                    freeNode(a);
                    freeNode(b);
                    freeNode(c);
					return NULL;
                }
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "add_expr";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "add_expr";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past cmp_expr(int t)
{
	past a = newAstNode();
	if((a = add_expr(t)))
	{
		past p = newAstNode();
		past pp = newAstNode();
		p->a = a;
		while(tok == CMP)
		{
			past b = newAstNode();
			past c = newAstNode();
			b = newKeyword(yypstr);
			advance();
			if(!(c = add_expr(t)))
			{
				if(t == 1)
				{
					printf("Expected an add_expr.\n");
					exit(-1);
				}
				else
                {
                    freeNode(p);
                    freeNode(pp);
                    freeNode(a);
                    freeNode(b);
                    freeNode(c);
					return NULL;
                }
			}
			pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "cmp_expr";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "cmp_expr";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past expr(int t)
{
	past p = newAstNode();
	past a = newAstNode();
	
	if((a = cmp_expr(t)))
	{
		p->a = a;
		p->nodeType = "Non-Terminal";
		p->svalue = "expr";
		return p;
	}
	freeNode(p);
	freeNode(a);
	return p;
}

past expression_statement(int t)
{
	past p = newAstNode();
	past a = newAstNode();
	past b = newAstNode();
	if(tok == ';')
	{
		a = newSymbol(";");
		advance();
		p->a = a;
		p->nodeType = "Non-Terminal";
		p->svalue = "empty-expression-statement";
		return p;
	}
	else if((a = expr(0)))
	{
		if(tok == ';')
		{
			b = newSymbol(";");
			advance();
			p->a = a;
			p->b = b;
			p->nodeType = "Non-Terminal";
			p->svalue = "expression-statement";
			return p;
		}
		else if(t == 1)
		{
			printf("Expected a ';'.\n");
			exit(-1);
		}
	}
	else if(t == 1)
	{
		printf("Expected correct Expression_statement.\n");
		exit(-1);
	}
	freeNode(a);
	freeNode(b);
	freeNode(p);
	return NULL;
}

past statement_list(int t)
{
    past l = newAstNode();
	past a = newAstNode();
	past pointer = newAstNode();
	if((a = statement(t)))
	{
		l->a = a;
		l->b = pointer;
		past ll = newAstNode();
		while((ll = statement(0))){
			pointer->nodeType = "Non-Terminal";
			pointer->svalue = "statements-node";
			pointer->a = ll;
			pointer->b = newAstNode();
			pointer = pointer->b;
			// ll->a = l->b;
			// l->b = ll;
		}
		l->nodeType = "Non-Terminal";
		l->svalue = "statement-list";
		return l;
	}
	else if(t == 1)
	{
		printf("Expected correct statement(list).\n");
		exit(-1);
	}
	freeNode(l);
	freeNode(a);
	return NULL;
}

past statement(int t)
{
	past p = newAstNode();
	past a = newAstNode();
	past b = newAstNode();
	past c = newAstNode();
	past d = newAstNode();
	past e = newAstNode();
	past f = newAstNode();
	past g = newAstNode();
	if((a = type(0)))
	{
		if((b = declarator_list(t)))
		{
			if(tok == ';')
			{
				freeNode(c);
				c = newSymbol(";");
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "declarator(s)-statement";
				advance();
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a ';'.\n");
				exit(-1);
			}
		}
		// else if(t == 1)
		// {
		// 	printf("Expected correct declarator_list.\n");
		// 	exit(-1);
		// }
	}
	else if(tok == '{')
	{
		freeNode(a);
		a = newSymbol("{");
		advance();
		if((b = statement_list(t)))
		{
			if(tok == '}')
			{
				freeNode(c);
				c = newSymbol("}");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "statement-list";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a '}'.\n");
				exit(-1);
			}
		}
		// else
	}
	else if((a = expression_statement(0)))
	{
		p->a = a;
		p->nodeType = "Non-Terminal";
		p->svalue = "expr-statement";
		return p;
	}
	else if(tok == IF)
	{
		a = newKeyword("if");
		advance();
		if(tok == '(')
		{
			b = newSymbol("(");
			advance();
			if((c = expr(t)))
			{
				if(tok == ')')
				{
					d = newSymbol(")");
					advance();
					if((e = statement(t)))
					{
						if(tok == ELSE)
						{
							f = newKeyword("else");
							advance();
							if((g = statement(t)))
							{
								p->a = a;
								p->b = b;
								p->c = c;
								p->d = d;
								p->e = e;
								p->f = f;
								p->g = g;
								p->nodeType = "Non-Terminal";
								p->svalue = "if-else-statement";
								return p;
							}
						}
						else
						{
							p->a = a;
							p->b = b;
							p->c = c;
							p->d = d;
							p->e = e;
							p->nodeType = "Non-Terminal";
							p->svalue = "if-statement";
							return p;
						}
					}
				}
				else if(t == 1)
				{
					printf("Expected a ')'.\n");
					exit(-1);
				}
			}
		}
		else if(t == 1)
		{
			printf("Expected a '('.\n");
			exit(-1);
		}
	}
	else if(tok == WHILE)
	{
		a = newKeyword("while");
		advance();
		if(tok == '(')
		{
			b = newSymbol("(");
			advance();
			if((c = expr(t)))
			{
				if(tok == ')')
				{
					d = newSymbol(")");
					advance();
					if((e = statement(t)))
					{
						p->a = a;
						p->b = b;
						p->c = c;
						p->d = d;
						p->e = e;
						p->nodeType = "Non-Terminal";
						p->svalue = "while-statement";
						return p;
					}
				}
				else if(t == 1)
				{
					printf("Expected a ')'.\n");
					exit(-1);
				}
			}
		}
		else if(t == 1)
		{
			printf("Expected a '('.\n");
			exit(-1);
		}
	}
	else if(tok == RETURN)
	{
		a = newKeyword("return");
		advance();
		if(tok == ';')
		{
			b = newSymbol(";");
			advance();
			p->a = a;
			p->b = b;
			p->nodeType = "Non-Terminal";
			p->svalue = "empty-return-statement";
			return p;
		}
		else if((b = expr(t)))
		{
			if(tok == ';')
			{
				c = newSymbol(";");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "return-statement";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a ';'.\n");
				exit(-1);
			}
		}
		else if(t == 1)
		{
			printf("Expected correct RETURN sentence.\n");
		}
	}
	else if(tok == PRINT)
	{
		a = newKeyword("print");
		advance();
		if(tok == ';')
		{
			b = newSymbol(";");
			advance();
			p->a = a;
			p->b = b;
			p->nodeType = "Non-Terminal";
			p->svalue = "empty-print-statement";
			return p;
		}
		else if((b = expr_list(t)))
		{
			if(tok == ';')
			{
				c = newSymbol(";");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "print-statement";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a ';'.\n");
				exit(-1);
			}
		}
		else if(t == 1)
		{
			printf("Expected correct PRINT sentence.\n");
			exit(-1);
		}
	}
	else if(tok == SCAN)
	{
		a = newKeyword("scan");
		advance();
		if((b = id_list(t)))
		{
			if(tok == ';')
			{
				c = newSymbol(";");
				advance();
				p->a = a;
				p->b = b;
				p->c = c;
				p->nodeType = "Non-Terminal";
				p->svalue = "scan-statement";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a ';'.\n");
				exit(-1);
			}
		}
		// else if(t == 1)
		// {
		// 	printf("Expected correct SCAN sentence.\n");
		// }
	}
	else if(t == 1)
	{
		printf("Expected correct Statement.\n");
		exit(-1);
	}
	freeNode(a);
	freeNode(b);
	freeNode(c);
	freeNode(d);
	freeNode(e);
	freeNode(f);
	freeNode(g);
	freeNode(p);
	return NULL;
}

past type(int t)
{
	past p = newAstNode();
	past a = newAstNode();//这里好像可以不要
	p->nodeType = "Non-Terminal";
	p->svalue = "type";
	if(tok == INT)
	{
		a = newKeyword("int");
		advance();
		p->a = a;
		return p;
	}
	else if(tok == STR)
	{
		a = newKeyword("str");
		advance();
		p->a = a;
		return p;
	}
	else if(tok == VOID)
	{
		a = newKeyword("VOID");		
		advance();
		p->a = a;
		return p;
	}
	else if(t == 1)
	{
		printf("Expected correct type.\n");
		exit(-1);
	}
	freeNode(p);
	freeNode(a);
	return NULL;
}

past parameter(int t)
{
	past p = newAstNode();
	past a = newAstNode();
	past b = newAstNode();
	if((a = type(t)))
	{
		if(tok == ID)
		{
			b = newId(yypstr);
			advance();
			p->a = a;
			p->b = b;
			p->nodeType = "Non-Terminal";
			p->svalue = "parameter";
			return p;
		}
		else if(t == 1)
		{
			printf("Expected correct parameter expression.\n");
			exit(-1);
		}
	}
	freeNode(a);
	freeNode(b);
	freeNode(p);
	return NULL;
}

past parameter_list(int t)
{
	past a = newAstNode();
	if((a = parameter(t)))
	{
        past p = newAstNode();
        past pp = newAstNode();
		past b = newAstNode();
		past c = newAstNode();
        p->a = a;
		while(tok == ',')
		{
			b = newSymbol(",");
			advance();
			if(!(c = parameter(t)))
			{
				if(t == 1)
				{
					printf("Expected a parameter.\n");
					exit(-1);
				}
				else
                {
                    freeNode(p);
                    freeNode(pp);
                    freeNode(a);
                    freeNode(b);
                    freeNode(c);
					return NULL;
                }
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "parameter_list";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "parameter-list";
		return p;//没少advance
	}
	freeNode(a);
	return NULL;
}

past declarator(int t)
{
    past p = newAstNode();
    past a = newAstNode();
    past b = newAstNode();
    past c = newAstNode();
    past d = newAstNode();
    past e = newAstNode();
    past f = newAstNode();
    past g = newAstNode();
    past h = newAstNode();
	if(tok == ID)
	{
        a = newId(yypstr);
		advance();
		if(tok == '=')
		{
            b = newSymbol("=");
			advance();
			if((c = expr(t)))
            {
                p->a = a;
                p->b = b;
                p->c = c;
                p->nodeType = "Non-Terminal";
                p->svalue = "assignment-declarator";
				return p;
            }
			else if (t == 1)
			{
				printf("Expected correct expr.\n");
				exit(-1);
			}
		}
		else if(tok == '(')
		{
            b = newSymbol("(");
			advance();
			if(tok == ')')
			{
                c = newSymbol("(");
				advance();
                p->a = a;
                p->b = b;
                p->c = c;
                p->nodeType = "Non-Terminal";
                p->svalue = "empty-bracket-declarator";
				return p;
			}
			else if((c = parameter_list(t)))
			{
				if(tok == ')')
				{
					d = newSymbol(")");
					advance();
					p->a = a;
					p->b = b;
					p->c = c;
					p->nodeType = "Non-Terminal";
					p->svalue = "parameter-list-declarator";
					return p;
				}
				else if(t == 1)
				{
					printf("Expected a ')'.\n");
					exit(-1);
				}
			}
			else if(t == 1)
			{
				printf("Expected a ')' or parameter_list.\n");
				exit(-1);
			}
		}
		else if(tok == '[')
		{
			b = newSymbol("[");
			advance();
			if(tok == ']')
			{
				c = newSymbol("]");
				advance();
				if(tok == '=')
				{
					d = newSymbol("=");
					advance();
					if(tok == '{')
					{
						e = newSymbol("{");
						advance();
						if((f = intstr_list(t)))
						{
							if(tok == '}')
							{
								g = newSymbol("}");
								advance();
								p->a = a;
								p->b = b;
								p->c = c;
								p->d = d;
								p->e = e;
								p->f = f;
								p->g = g;
								p->nodeType = "Non-Terminal";
								p->svalue = "array-assignment-without-expr-declarator";
								return p;
							}
							else if(t == 1)
							{
								printf("Expected a '}'.\n");
								exit(-1);
							}
						}
						else if(t == 1)
						{
							printf("Expected correct intstr_list.\n");
							exit(-1);
						}
					}
					else if(t == 1)
					{
						printf("Expected '{' intstr_list '}'.\n");
						exit(-1);
					}
				}
				else
				{
					p->a = a;
					p->b = b;
					p->c = c;
					p->nodeType = "Non-Terminal";
					p->svalue = "array-without-expr-assignment";
					return p;//没少advance
				}
			}
			else if((c = expr(t)))
			{
				if(tok == ']')
				{
					d = newSymbol("]");
					advance();
					if(tok == '=')
					{
						e = newSymbol("=");
						if(tok == '{')
						{
							f = newSymbol("{");
							advance();
							if((g = intstr_list(t)))
							{
								if(tok == '}')
								{
									h = newSymbol("}");
									advance();
									p->a = a;
									p->b = b;
									p->c = c;
									p->d = d;
									p->e = e;
									p->f = f;
									p->g = g;
									p->h = h;
									p->nodeType = "Non-Terminal";
									p->svalue = "array-assignment-declarator";
									return p;
								}
								else if(t == 1)
								{
									printf("Expected a '}'.\n");
									exit(-1);
								}
							}
						}
					}
					else
					{
						p->a = a;
						p->b = b;
						p->c = c;
						p->d = d;
						p->nodeType = "Non-Terminal";
						p->svalue = "array-declarator";
						return p;//没少advance
					}
				}
			}
			// else
			// 	if(t == 1)
			// 		printf("Expected correct declarator expression.\n");
		}
		else
		{
			p->a = a;
			p->nodeType = "Non-Terminal";
			p->svalue = "ID-declarator";
			return p;
		}
	}
	else if(t == 1)
	{
		printf("Expected correct declarator expression.\n");
		exit(-1);
	}
    freeNode(p);
    freeNode(a);
    freeNode(b);
    freeNode(c);
    freeNode(d);
    freeNode(e);
    freeNode(f);
    freeNode(g);
    freeNode(h);
	return 0;
}

past initializer(int t)
{
    past p = newAstNode();
	if(tok == NUMBER)
	{
        p = newNumber(yylval);
        // p->nodeType = "initializer-NUMBER";
        // p->ivalue = yylval;
		advance();
		return p;
	}
	else if(tok == STRING)
	{
        p = newString(yypstr);
        // p->nodeType = "initializer-STRING";
        // p->svalue = yypstr;
		advance();
		return p;
	}
	else if(t == 1)
	{
		printf("Expected a number or a string.\n");
		exit(-1);
	}
    freeNode(p);
	return NULL;
}

past intstr_list(int t)
{
    past a = newAstNode();
	if((a = initializer(t)))
	{
        past p = newAstNode();
        past pp = newAstNode();
        p->a = a;
		while (tok == ',')
		{
            past b = newAstNode();
            past c = newAstNode();
            b = newSymbol(",");
			advance();
			if(!(c = initializer(t)))
			{
				if(t == 1)
				{
					printf("Expected an initializer.\n");
					exit(-1);
				}
				else
                {
                    freeNode(p);
                    freeNode(pp);
                    freeNode(a);
                    freeNode(a);
                    freeNode(c);
					return NULL;
                }
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "intstr_list";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "intstr-list";

		return p;//没少advance
	}
    freeNode(a);
	return NULL;
}

past declarator_list(int t)
{
    past a = newAstNode();
	if((a = declarator(t)))
	{
        past p = newAstNode();
        past pp = newAstNode();
        p->a = a;
		while (tok == ',')
		{
            past b = newAstNode();
            past c = newAstNode();
            b = newSymbol(",");
			advance();
			if(!(c = declarator(t)))
			{
				if(t == 1)
				{ 
					printf("Expected a declarator.\n");
					exit(-1);
				}
				else
                {
                    freeNode(p);
                    freeNode(pp);
                    freeNode(a);
                    freeNode(b);
                    freeNode(c);
					return NULL;
                }
			}
            pp->a = p->b;
            pp->b = b;
            pp->c = c;
            pp->nodeType = "Non-Terminal";
            pp->svalue = "declarator_list";
            p->b = pp;
		}
		p->nodeType = "Non-Terminal";
		p->svalue = "declarator_main_list";
		return p;//没少advance
	}
    freeNode(a);
	return NULL;
}

past decl_or_stmt(int t)
{
    past p = newAstNode();
    past a = newAstNode();
    past b = newAstNode();
    past c = newAstNode();
	if(tok == '{')
	{
        a = newSymbol("{");
		advance();
		if(tok == '}')
		{
            b = newSymbol("}");
			advance();
            p->a = a;
            p->b = b;
            freeNode(c);
            p->nodeType = "Non-Terminal";
            p->svalue = "empty_decl_or_stmt";
			return p;
		}
		else if((b = statement_list(t)))
		{
			// advance();
			if(tok == '}')
			{
                c = newSymbol("}");
				advance();
                p->a = a;
                p->b = b;
                p->c = c;
                p->nodeType = "Non-Terminal";
                p->svalue = "stmt";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a '}'.\n");
				exit(-1);
			}
		}
		else if(t == 1)
		{
			printf("Expected a '}' or statement_list.\n");
			exit(-1);
		}
	}
	else if(tok == ',')
	{
        a = newSymbol(",");
		advance();
		if((b = declarator_list(t)))
		{
			// advance();
			if(tok == ';')
			{
                c = newSymbol(";");
				advance();
                p->a = a;
                p->b = b;
                p->c = c;
                p->nodeType = "Non-Terminal";
                p->svalue = "decl";
				return p;
			}
			else if(t == 1)
			{
				printf("Expected a ';'.\n");
				exit(-1);
			}
		}
		else if(t == 1)
		{
			printf("Expected correct declarator_list.\n");
			exit(-1);
		}
	}
	else if(tok == ';')
	{
		advance();
        p->nodeType = "Semi";
        p->svalue = ";";
        freeNode(a);
        freeNode(b);
        freeNode(c);
		return p;
	}
    freeNode(a);
    freeNode(b);
    freeNode(c);
    freeNode(p);
	return NULL;
}

past external_declaration(int t)
{
    past p = newAstNode();
    past a = newAstNode();
    past b = newAstNode();
    past c = newAstNode();
	if((a = type(t)))
	{
		// free(b);
		if((b = declarator(t)))
		{
			if((c = decl_or_stmt(t)))
			{
                p->a = a;
                p->b = b;
                p->c = c;
                p->nodeType = "Non-Terminal";
                p->svalue = "external_declaration";
				return p;
			}
		}
	}
	if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
    freeNode(a);
    freeNode(b);
    freeNode(c);
    freeNode(p);
	return NULL;
}

past program(int t)//to finish!!!//finished!without solving problem in the comment below
{
    past l = newAstNode();
	past a = newAstNode();
	past pointer = newAstNode();
	if((a = external_declaration(t)))
	{
		l->a = a;
		l->b = pointer;
		past ll = newAstNode();
		while((ll = external_declaration(0))){
			pointer->nodeType = "Non-Terminal";
			pointer->svalue = "external_declarations-node";
			pointer->a = ll;
			pointer->b = newAstNode();
			pointer = pointer->b;
			// ll->a = l->b;
			// l->b = ll;
		}//似乎存在潜在?隐患：ext_decl+ext_decl+type+decl时，yylex无法退回
		freeNode(pointer);
		l->nodeType = "Non-Terminal";
		l->svalue = "program";
		return l;//各list可能也有此隐患，记得处理
	}
	else if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
	freeNode(l);
	freeNode(a);
	freeNode(pointer);
	return NULL;
}



void showAst(past node, int nest)
{
    // if(node == NULL)
	if(node == NULL ||(node->nodeType == NULL && node->a == NULL))
        return;

    int i = 0;
    for(i = 0; i < nest; i ++)
        printf("  ");
    if(strcmp(node->nodeType, "NUMBER") == 0)
        printf("%s : %d\n", node->nodeType, node->ivalue);
    else// if(strcmp(node->nodeType, "expr") == 0)
        printf("%s : %s\n", node->nodeType, node->svalue);
    showAst(node->a, nest+1);
    showAst(node->b, nest+1);//unverification!
    showAst(node->c, nest+1);//unverification!
    showAst(node->d, nest+1);//unverification!
    showAst(node->e, nest+1);//unverification!
    showAst(node->f, nest+1);//unverification!
    showAst(node->g, nest+1);//unverification!
    showAst(node->h, nest+1);

}

int main(int argc, char **argv)
{
	// while(1)
	// {
	printf("Reading file:\n");
	// advance();
	setbuf(stdout,NULL);
	yyin = fopen("test.c", "r");
	advance();
	// int r = program(1);
	past rr = program(1);
	showAst(rr, 0);
	if(rr)
		printf("Grammar correct!\n");
	else
		printf("Grammar incorrect!\n");

		// past rr = astExpr();
		// showAst(rr, 0);
	// }
    free(yypstr);
	return 0;
}
