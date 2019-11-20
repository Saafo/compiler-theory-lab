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


int factor();
int expr();
int term();

int tok;

void advance()
{
    tok = yylex();
    printf("tok: %s\n", yytext);
}


typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
    union{
        int ivalue;
        char* svalue;
    };
    char* nodeType;
    past left;
    past middle;
    past right;
};


past newAstNode()
{
    past node = malloc(sizeof(ast));
    if(!node)
    {
        printf("run out of memory.\n");
        exit(-2);
    }
    memset(node, 0, sizeof(ast));
    return node;
}

void freeNode(past p)
{
    if(p->left)
        freeNode(p->left);
    if(p->middle)
        freeNode(p->middle);
    if(p->right)
        freeNode(p->right);
    free(p);
}


past newNumber(int value)
{
    past var = newAstNode();
    var->ivalue = value;
    var->nodeType = "NUMBER";
    return var;
}

past newOperator(int oper, past left, past right)
{
    past var = newAstNode();
    var->ivalue = oper;
    var->nodeType = "OPERATOR";
    var->left = left;
    var->right = right;
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

past decl_or_stmt(int t)
{
    past p = newAstNode();
    past l = newAstNode();
    past r = newAstNode();
    past m = newAstNode();
	if(tok == '{')
	{
        l = newSymbol("{");
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

past external_declaration(int t)
{
    past p = newAstNode();
    past l = newAstNode();
    past r = newAstNode();
    past m = newAstNode();
	if((l = type(t)))
	{
		if((m = declarator(t)))
		{
			if((r = decl_or_stmt(t)))
			{
                p->left = l;
                p->middle = m;
                p->right = r;
				return p;
			}
		}
	}
	if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
    freeNode(l);
    freeNode(r);
    freeNode(m);
    freeNode(p);
	return NULL;
}

past astProgram(int t)//to finish!!!
{
    past l;
	if((l = external_declaration(t)))
	{
		while(external_declaration(0)){}//似乎存在潜在?隐患：ext_decl+ext_decl+type+decl时，yylex无法退回
		return l;//各list可能也有此隐患，记得处理
	}
	else if(t == 1)
	{
		printf("Expected correct external_declaration.\n");
		exit(-1);
	}
	return 0;
}



void showAst(past node, int nest)
{
    if(node == NULL)
        return;

    int i = 0;
    for(i = 0; i < nest; i ++)
        printf("  ");
    if(strcmp(node->nodeType, "nValue") == 0)
        printf("%s %d\n", node->nodeType, node->ivalue);
    else if(strcmp(node->nodeType, "expr") == 0)
        printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
    showAst(node->left, nest+1);
    showAst(node->middle,nest+1);//unverification!
    showAst(node->right, nest+1);

}
