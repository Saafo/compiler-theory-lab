/*
 ============================================================================
 Name        : rdparser.c
 Author      : @saafo
 Version     : v0.1 Pre Alpha
 Copyright   : Copyright (c) 2019 SaffordHuang All rights reserved.
 Description : A Checker
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
// principles:
//1. advance()写在函数内部，return 1时要注意需不需要advance()
//2. 回归旧的设计模式：恢复函数参数，print只在函数开始匹配东西后输出，并exit(-1),若函数未匹配则return 0;
//3. 新发现：if(func())失败了不用printf和exit
//--------------------------
//记录一下：本来最开始采用参数控制是否要输出并退出，但后面以为这样很麻烦，
//只需要匹配第一个模块是否能匹配就好了，但直到statement()的时候发现第一个参数
//也不能确定是否就一定匹配该函数，所以新建了nonparameter分支保留了当时的痕迹
//再次开始带参数的工作
//--------------------------
// return value:
// 0:failed
// 1:success
// parameter(allow print error):
// 0:false
// 1:true(forced)
//--------------------------


// program
//     : external_declaration
//     | program external_declaration
//     ;
// to clear left recursion:

int program(int t)
{
	if(external_declaration(t))
	{
		while(external_declaration(0)){}//似乎存在潜在?隐患：ext_decl+ext_decl+type+decl时，yylex无法退回
		return 1;//各list可能也有此隐患，记得处理
	}
	else if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
	return 0;
}

// external_declaration
//     : type declarator decl_or_stmt
//     ;

int external_declaration(int t)
{
	if(type(t))
	{
		// advance();
		if(declarator(t))
		{
			// advance();
			if(decl_or_stmt(t))
			{
				// advance();
				return 1;
			}
		}
	}
	if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
	return 0;
}

// decl_or_stmt
//     : '{' statement_list '}'
//     | '{' '}'
//     | ',' declarator_list ';'
//     | ';'
//     ;

int decl_or_stmt(int t)
{
	if(tok == '{')
	{
		advance();
		if(tok == '}')
		{
			advance();
			return 1;
		}
		else if(statement_list(t))
		{
			// advance();
			if(tok == '}')
			{
				advance();
				return 1;
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
		advance();
		if(declarator_list(t))
		{
			// advance();
			if(tok == ';')
			{
				advance();
				return 1;
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
		return 1;
	}
	return 0;
}

// declarator_list
//     : declarator
//     | declarator_list ',' declarator
//     ;

int declarator_list(int t)
{
	if(declarator(t))
	{
		while (tok == ',')
		{
			advance();
			if(!declarator(t))
			{
				if(t == 1)
				{
					printf("Expected a declarator.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;//没少advance
	}
	return 0;
}

// intstr_list
//     : initializer
//     | intstr_list ',' initializer
//     ;

int intstr_list(int t)
{
	if(initializer(t))
	{
		while (tok == ',')
		{
			advance();
			if(!initializer(t))
			{
				if(t == 1)
				{
					printf("Expected an initializer.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;//没少advance
	}
	return 0;
}

// initializer
//     : NUMBER
//     | STRING
//     ;

int initializer(int t)
{
	if(tok == NUMBER)
	{
		advance();
		return 1;
	}
	else if(tok == STRING)
	{
		advance();
		return 1;
	}
	else if(t == 1)
	{
		printf("Expected a number or a string.\n");
		exit(-1);
	}
	return 0;
}

// declarator
//     : ID
//     | ID '=' expr
//     | ID '(' parameter_list ')'
//     | ID '(' ')'
//     | ID '[' ']' '=' '{' intstr_list '}'
//     | ID '[' ']'
//     | ID '[' expr ']' '=' '{' intstr_list '}'
//     | ID '[' expr ']'
//     ;

int declarator(int t)
{
	if(tok == ID)
	{
		advance();
		if(tok == '=')
		{
			advance();
			if(expr(t))
				return 1;
			else if (t == 1)
			{
				printf("Expected correct expr.\n");
				exit(-1);
			}
		}
		else if(tok == '(')
		{
			advance();
			if(tok == ')')
			{
				advance();
				return 1;
			}
			else if(parameter_list(t))
			{
				if(tok == ')')
				{
					advance();
					return 1;
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
			advance();
			if(tok == ']')
			{
				advance();
				if(tok == '=')
				{
					advance();
					if(tok == '{')
					{
						advance();
						if(intstr_list(t))
						{
							if(tok == '}')
							{
								advance();
								return 1;
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
					return 1;//没少advance
			}
			else if(expr(t))
			{
				if(tok == ']')
				{
					advance();
					if(tok == '=')
					{
						if(tok == '{')
						{
							advance();
							if(intstr_list(t))
							{
								if(tok == '}')
								{
									advance();
									return 1;
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
						return 1;//没少advance
				}
			}
			// else
			// 	if(t == 1)
			// 		printf("Expected correct declarator expression.\n");
		}
		else
			return 1;
	}
	else if(t == 1)
	{
		printf("Expected correct declarator expression.\n");
		exit(-1);
	}
	return 0;
}

// parameter_list
//         : parameter
//         | parameter_list ',' parameter
//         ;

int parameter_list(int t)
{
	if(parameter(t))
	{
		while(tok == ',')
		{
			advance();
			if(!parameter(t))
			{
				if(t == 1)
				{
					printf("Expected a parameter.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;//没少advance
	}

	return 0;
}

// parameter
//         : type ID
//         ;

int parameter(int t)
{
	if(type(t))
	{
		if(tok == ID)
		{
			advance();
			return 1;
		}
		else if(t == 1)
		{
			printf("Expected correct parameter expression.\n");
			exit(-1);
		}
	}
	return 0;
}

// type
//         : INT
//         | STR
//         | VOID
//         ;

int type(int t)
{
	if(tok == INT)
	{
		advance();
		return 1;
	}
	else if(tok == STR)
	{
		advance();
		return 1;
	}
	else if(tok == VOID)
	{
		advance();
		return 1;
	}
	else if(t == 1)
	{
		printf("Expected correct type.\n");
		exit(-1);
	}
	return 0;
}

// statement
//     : type declarator_list ';'
//     | '{' statement_list '}'
//     | expr_statement
//     | IF '(' expr ')' statement ELSE statement
//     | IF '(' expr ')' statement
//     | WHILE '(' expr ')' statement
//     | RETURN ';'
//     | RETURN expr ';'
//     | PRINT ';
//     | PRINT expr_list ';'
//     | SCAN id_list ';'
//     ;

int statement(int t)
{
	if(type(0))
	{
		if(declarator_list(t))
		{
			if(tok == ';')
			{
				advance();
				return 1;
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
		advance();
		if(statement_list(t))
		{
			if(tok == '}')
			{
				advance();
				return 1;
			}
			else if(t == 1)
			{
				printf("Expected a '}'.\n");
				exit(-1);
			}
		}
		// else
	}
	else if(expression_statement(0))
		return 1;
	else if(tok == IF)
	{
		advance();
		if(tok == '(')
		{
			advance();
			if(expr(t))
			{
				if(tok == ')')
				{
					advance();
					if(statement(t))
					{
						if(tok == ELSE)
						{
							advance();
							if(statement(t))
								return 1;
						}
						else
							return 1;
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
		advance();
		if(tok == '(')
		{
			advance();
			if(expr(t))
			{
				if(tok == ')')
				{
					advance();
					if(statement(t))
						return 1;
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
		advance();
		if(tok == ';')
		{
			advance();
			return 1;
		}
		else if(expr(t))
		{
			if(tok == ';')
			{
				advance();
				return 1;
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
		advance();
		if(tok == ';')
		{
			advance();
			return 1;
		}
		else if(expr_list(t))
		{
			if(tok == ';')
			{
				advance();
				return 1;
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
		advance();
		if(id_list(t))
		{
			if(tok == ';')
			{
				advance();
				return 1;
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
	return 0;
}

// statement_list
//     : statement
//     | statement_list statement
//     ;

int statement_list(int t)
{
	if(statement(t))
	{
		while(statement(0)){}
		return 1;
	}
	else if(t == 1)
	{
		printf("Expected correct statement(list).\n");
		exit(-1);
	}
	return 0;
}

// expression_statement
//     : ';'
//     | expr ';'
//     ;

int expression_statement(int t)
{
	if(tok == ';')
	{
		advance();
		return 1;
	}
	else if(expr(0))
	{
		if(tok == ';')
		{
			advance();
			return 1;
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
	return 0;
}

// expr
//     : cmp_expr
//     ;

int expr(int t)
{
	if(cmp_expr(t))
	{
		return 1;
	}
	return 0;
}

// cmp_expr
//     : add_expr
//     | cmp_expr CMP add_expr
//     ;

int cmp_expr(int t)
{
	if(add_expr(t))
	{
		while(tok == CMP)
		{
			advance();
			if(!add_expr(t))
			{
				if(t == 1)
				{
					printf("Expected an add_expr.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;//没少advance
	}
	return 0;
}

// add_expr
//     : mul_expr
//     | add_expr '+' mul_expr
//     | add_expr '-' mul_expr
//     ;

int add_expr(int t)
{
	if(mul_expr(t))
	{
		while (tok == '+' || tok == '-')
		{
			advance();
			if(!mul_expr(t))
			{
				if(t == 1)
				{
					printf("Expected mul_expr.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;//没少advance()
	}
	return 0;
}

// mul_expr
//     : primary_expr
//     | mul_expr '*' primary_expr
//     | mul_expr '/' primary_expr
//     | mul_expr '%' primary_expr
//     | '-' primary_expr
//     ;

int mul_expr(int t)
{
	if(tok == '-')
		advance();
	if(primary_expr(t))
	{
		while(tok == '*' || tok == '/' || tok == '%')
		{
			advance();
			if(tok == '-')
				advance();
			if(!primary_expr(t))
			{
				if(t == 1)
				{
					printf("Expected primary_expr.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;
	}
	return 0;
}

// primary_expr
//     : ID '(' expr_list ')'
//     | ID '(' ')'
//     | ID ASSIGN expr
//     | ID '=' expr
//     | ID '[' expr ']'
//     | ID '[' expr ']' '=' expr
//     | ID
//     | NUMBER
//     | STRING
//     | '(' expr ')'
//     ;

int primary_expr(int t)
{
	if(tok == ID)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if(expr_list(t))
			{
				if(tok == ')')
				{
					advance();
					return 1;
				}
				else if(t == 1)
				{
					printf("Expected a ')'.\n");
					exit(-1);
				}
				else
					return 0;
			}
			else if (tok == ')')
			{
				advance();
				return 1;
			}
			else if(t == 1)
			{
				printf("Expected ')' or expr_list.\n");
			}
			else
				return 0;
		}
		else if (tok == ASSIGN)
		{
			advance();
			if(expr(t))
			{
				return 1;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return 0;
		}
		else if (tok == '=')
		{
			advance();
			if(expr(t))
			{
				return 1;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return 0;
		}
		else if (tok == '[')
		{
			advance();
			if(expr(t))
			{
				if(tok == ']')
				{
					advance();
					if(tok == '=')
					{
						if(expr(t))
						{
							return 1;
						}
						else if (t == 1)
						{
							printf("Expected expr.\n");
							exit(-1);
						}
						else
							return 0;
					}
					else
						return 1;
				}
				else if (t == 1)
				{
					printf("Expected a ']'.\n");
					exit(-1);
				}
				else
					return 0;
			}
			else if (t == 1)
			{
				printf("Expected expr.\n");
				exit(-1);
			}
			else
				return 0;
		}
		else
			return 1;
	}	
	else if (tok == NUMBER)
	{
		advance();
		return 1;
	}
	else if (tok == STRING)
	{
		advance();
		return 1;
	}
	else if (tok == '(')
	{
		advance();
		if(expr(t))
		{
			if(tok == ')')
			{
				advance();
				return 1;
			}
			else if (t == 1)
			{
				printf("Expected a ')'.\n");
				exit(-1);
			}
			else
				return 0;
		}
		else if (t == 1)
		{
			printf("Expected expr.\n");
			exit(-1);
		}
		else
			return 0;
	}
	else
		return 0;
}

// expr_list
//     : expr
//     | expr_list ',' expr
//     ;

int expr_list(int t)
{
	if(expr(t))
	{
		while(tok == ',')
		{
			advance();
			if(!expr(t))
			{
				if(t == 1)
				{
					printf("Expected an expr.\n");
					exit(-1);
				}
				else
					return 0;
			}
		}
		return 1;
	}
	return 0;
}

// id_list
//     : ID
//     | id_list ',' ID
//     ;

int id_list(int t)
{
	if(tok == ID)
	{
		advance();
		while(tok == ',')
		{
			advance();
			if(tok != ID)
			{
				if(t == 1)
				{
					printf("Expected an ID.\n");
					exit(-1);
				}
				else
					return 0;
			}
			advance();
		}
		return 1;
	}
	return 0;
}

//-----------------------------------------------

int main(int argc, char **argv)
{
	// while(1)
	// {
	printf("Reading file:\n");
	// advance();
	setbuf(stdout,NULL);
	yyin = fopen("test.c", "r");
	advance();
	int r = program(1);
	if(r)
		printf("Grammar correct!\n");
	else
		printf("Grammar incorrect!\n");

		//past rr = astExpr();
		//showAst(rr, 0);
	// }
    free(yypstr);
	return 0;
}
