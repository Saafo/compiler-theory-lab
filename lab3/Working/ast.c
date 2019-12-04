#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int lineno;

//

/*
struct _ast{
    char *nodeType;
    int line;
    int value;
    char *str;
    past l;
    past r;
    past next;
};
*/
//left 和 right 分别指向运算分量的两个结点

/*

*/
past newAstNode()
{
    past p = (past)malloc(sizeof(ast));
    if (p == NULL)
    {
        printf("New astNode failed.\n");
        exit(0);
    }
    memset(p, 0, sizeof(ast));
    p->line = lineno;
    p->l = NULL;
    p->r = NULL;
    return p;
}

past newList(past list,past node){
    if(list !=NULL){
    list->r->next = node;//将新结点添加到链尾
    list->r =node;//尾指针移动到链尾
    list->value +=1;
    return list;    
    }
    //创建一个list
    list = newAstNode();
    list->next = node;
    list ->value = 1;
    list ->l = node;
    list ->r = node;
    return list;
}
/*
//针对左递归文法
past newList(past list,past node){
    if(!list != NULL){

    }
}
*/

past newID(char *sval)
{
    past p = newAstNode();
    p->nodeType = "ID";
    p->str = sval;
    return p;
}

past newNUMBER(int ival)
{
    past p = newAstNode();
    p->nodeType = "NUMBER";
    p->value = ival;
    return p;
}

past newSTRING(char *sval)
{
    past p = newAstNode();
    p->nodeType = "STRING";
    p->str = sval;
    return p;
}

past newExpr(past l, int oper, past r)
{
    past p = newAstNode();
    p->nodeType = "expr";
    p->value = oper;
    p->l = l;
    p->r = r;
    return p;
}

/*
    1 program: external_declaration
    2        | program external_declaration
*/
past program(past l, past r){
	past list = newList(l, r);
	list->nodeType = "program";
	return list;
}
/*
    3 external_declaration: function_definition
    4                     | declaration
*/
past ext_decl(past l){
	past p = newAstNode();
	p->l = l;
	p->nodeType = "ext_decl";
	return p;
}
/*5 function_definition: type declarator compound_statement*/
past func_def(past type, past declr, past com_stmt){
	past p = newAstNode();
	p->nodeType = "func_def";
	p->l = type;
	p->r = declr;
	p->str = (char *)com_stmt; //如果用next保存结点，输出语法树时不同的extDecl会混合
	return p;
}
/*
    6 declaration: type init_declarator_list ';'
*/
past decln(past l, past r){
	//这里写成declr更合适？x下面有declr
	past p = newAstNode();
	p->nodeType = "decln";
	p->l = l;
	p->r = r;
	return p;
}
/*
    7 init_declarator_list: init_declarator
    8                     | init_declarator_list ',' init_declarator
*/
past init_declr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "init_declr_list";
	return list;
}
/*
  16 declarator: direct_declarator
*/
past declr(past l, past r){
	past p = newAstNode();
	p->nodeType = "declr";
	p->l = l;
	p->r = r;
	return p;
}
/*
   12 intstr_list: initializer
   13            | intstr_list ',' initializer
*/
past intstr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "intstr_list";
	return list;
}
/*
   17 direct_declarator: ID
   18                  | direct_declarator '(' parameter_list ')'
   19                  | direct_declarator '(' ')'
   20                  | ID '[' expr ']'
   21                  | ID '[' ']'
*/
past dir_declr(past l, past r){
	past p = newAstNode();
	p->nodeType = "dir_declr";
	p->l = l;
	p->r = r;
	return p;
}
/*
   22 parameter_list: parameter
   23               | parameter_list ',' parameter
*/
past para_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "para_list";
	return list;
}
/*
 24 parameter: type ID
 parameter->参数
*/
past parameter(past l, past r){
	past p = newAstNode();
	p->nodeType = "parameter";
	p->l = l;
	p->r = r;
	return p;
}

/*
   25 type: INT
   26     | STR
   27     | VOID
*/
past type(char *t){
	past p = newAstNode();
	p->nodeType = "type";
	p->str = t;
	return p;
}
/*
   36 compound_statement: begin_scope end_scope
   37                   | begin_scope statement_list end_scope
compound->复合
*/
past com_stmt(past l, past r, past s){
	past p = newAstNode();
	p->nodeType = "com_stmtpats";
	p->l = l;
	p->r = r;
	p->str = (char *)s; //如果用next保存结点，输出语法树时不同的extDecl会混合
	return p;
}
/*
38 begin_scope: '{'
scope:作用域
*/
past begin_scope(){
	past p = newAstNode();
	p->nodeType = "begin_scope";
	return p;
}
/*
end_scope: '}'
*/
past end_scope(){
	past p = newAstNode();
	p->nodeType = "end_scope";
	return p;
}
/*
   40 statement_list: statement
   41               | statement_list statement
   左递归语句
*/
past stmt_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "stmt_list";
	return list;
}
/*
   42 expression_statement: ';'
   43                     | expr ';'

*/
past expr_stmt(past l){
	past p = newAstNode();
	p->l = l;
	p->nodeType = "expr_stmt";
	return p;
}
/*
   44 selection_statement: IF '(' expr ')' statement
   45                    | IF '(' expr ')' statement ELSE statement
*/
past if_stmt(past cond, past trueStmt, past falseStmt){
	past p = newAstNode();
	p->nodeType = "if_stmt";
	p->l = trueStmt;
	p->r = falseStmt;
	p->str = (char *)cond;
	return p;
}
/*
   46 iteration_statement: WHILE '(' expr ')' statement
*/
past while_stmt(past cond, past stmt){
	past p = newAstNode();
	p->nodeType = "while_stmt";
	p->l = cond;
	//cond->condition
	p->r = stmt;
	return p;
}
/*
   47 jump_statement: RETURN ';'
   48               | RETURN expr ';'

*/
past return_stmt(past expr){
	past p = newAstNode();
	p->nodeType = "return_stmt";
	p->l = expr;
	return p;
}
/*
   49 print_statement: PRINT ';'
   50                | PRINT expr_list ';'

*/
past print_stmt(past expr_list){
	past p = newAstNode();
	p->nodeType = "print_stmt";
	p->l = expr_list;
	return p;
}
/*
51 scan_statement: SCAN id_list ';'
*/
past scan_stmt(past id_list){
	past p = newAstNode();
	p->nodeType = "scan_stmt";
	p->l = id_list;
	return p;
}
/*
   53 assign_expr: cmp_expr
   54            | ID ASSIGN assign_expr
   55            | ID '=' assign_expr
   56            | ID '[' expr ']' '=' assign_expr

*/
past assign_expr(int ival, past l, past r, past s){
	past p = newAstNode();
	p->nodeType = "assign_expr";
	p->value = ival;
	p->l = l;
	p->r = r;
	p->str = (char *)s;
	return p;
}
/*
   57 cmp_expr: add_expr
   58         | cmp_expr CMP add_expr
*/
past cmp_expr(past l, int ival, past r){
	past p = newAstNode();
	p->nodeType = "cmp_expr";
	p->value = ival;
	p->l = l;
	p->r = r;
	return p;
}
/*
   67 primary_expr: ID '(' expr_list ')'
   68             | ID '(' ')'
   69             | '(' expr ')'
   70             | ID
   71             | initializer
   72             | ID '[' expr ']'
*/
past primary_expr(past l, past r){
	past p = newAstNode();
	p->nodeType = "primary_expr";
	p->l = l;
	p->r = r;
	return p;
}
/*
   73 expr_list: expr
   74          | expr_list ',' expr
*/
past expr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "expr_list";
	return list;
}
/*
   75 id_list: ID
   76        | id_list ',' ID 
*/
past id_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "id_list";
	return list;
}
/*
没用到的语法规则：
    9 init_declarator: declarator
   10                | declarator '=' add_expr
   11                | declarator '=' '{' intstr_list '}'

   14 initializer: NUMBER
   15            | STRING

   28 statement: compound_statement
   29          | expression_statement
   30          | selection_statement
   31          | iteration_statement
   32          | jump_statement
   33          | print_statement
   34          | scan_statement
   35          | declaration

   52 expr: assign_expr

   59 add_expr: mul_expr
   60         | add_expr '+' mul_expr
   61         | add_expr '-' mul_expr

   62 mul_expr: primary_expr
   63         | mul_expr '*' primary_expr
   64         | mul_expr '/' primary_expr
   65         | mul_expr '%' primary_expr
   66         | '-' primary_expr

//Multiplication expression
*/
void showAst(past p,int nest){
    if(p == NULL)
        return;
    int i = 0;
    for(i = 0;i<nest;i++)
        printf("  ");
    if(strcmp(p->nodeType,"ID")==0)
        printf("id:%s\n",p->str);
    else if(strcmp(p->nodeType,"NUMBER")==0)
        printf("num:%d\n",p->value);
    else if(strcmp(p->nodeType,"STRING")==0)
        printf("str:%s\n",p->str);
    else if(strcmp(p->nodeType,"program")==0){
        printf("%s\n",p->nodeType);
        //?
        past t = p->l;
        int i =1;
        for(;i<=p->value;i++){
            showAst(t,nest+1);
            t = t->next;
        }    
    }
    else if(strcmp(p->nodeType,"ext_decl")==0){
        printf("ext_decl\n");
        showAst(p->l,nest+1);
    }
    else if(strcmp(p->nodeType,"func_def")==0){
        printf("func_def\n");
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
        showAst((past)p->str,nest+1);
    }
    else if(strcmp(p->nodeType,"decln")==0){
        printf("%s\n",p->nodeType);
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
    }
    else if(strcmp(p->nodeType,"init_declr_list")==0){
        printf("%s\n",p->nodeType);
        past t = p->l;
        int i=1;
        for(;i<=p->value;i++){
            showAst(t,nest+1);
            t=t->next;
        }
    }
    else if (strcmp(p->nodeType,"declr")==0){
        printf("%s\n",p->nodeType);
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
    }
    else if(strcmp(p->nodeType,"intstr_list")==0){
        printf("%s\n",p->nodeType);
        past t = p->l;
        int i =1;
        for(;i<=p->value;i++){
            showAst(t,nest+1);
            t = t->next;
        }
    }
    else if(strcmp(p->nodeType,"dir_declr")==0){
        showAst(p->l,0);
        showAst(p->r,nest);
    }
    //每次展开list让它作为当前的root
    else if(strcmp(p->nodeType,"para_list")==0){
        printf("%s\n",p->nodeType);
        past t=p->l;
        int i = 1;
        for(;i<=p->value;i++){
            showAst(t,nest);
            t=t->next;
        }
    }
    else if(strcmp(p->nodeType,"parameter")==0){
        printf("%s\n",p->nodeType);
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
    }
    else if(strcmp(p->nodeType,"type")==0){
        printf("%s:%s\n",p->nodeType,p->str);
    }
    else if(strcmp(p->nodeType,"com_stmt")==0){
        printf("compound_statement\n");
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
        showAst((past)p->str,nest+1);
    }
    else if(strcmp(p->nodeType,"begin_scope")==0){
        printf("\n");
    }
    else if(strcmp(p->nodeType,"end_scope")==0){
        printf("\n");
    }
    else if(strcmp(p->nodeType,"stmt_list")==0){
        printf("%s\n",p->nodeType);
        past t =p->l;
        int i=1;
        for(;i<=p->value;i++){
            showAst(t,nest);
            t=t->next;
        }
    }
    else if(strcmp(p->nodeType,"expr_stmt")==0){
        printf("%s\n",p->nodeType);
        showAst(p->l,nest+1);
    }
    else if(strcmp(p->nodeType,"if_stmt")==0){
        printf("%s\n",p->nodeType);
        showAst((past)p->str,nest+1);
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
    }
    else if(strcmp(p->nodeType,"while_stmt")==0){
        printf("%s\n",p->nodeType);
        showAst(p->l,nest+1);
        showAst(p->r,nest+1);
    }
    else if(strcmp(p->nodeType,"return_stmt")==0){
        printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
    }
    else if(strcmp(p->nodeType,"print_stmt")==0){
        printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
    }
    else if(strcmp(p->nodeType,"scan_stmt")==0){
        printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
    }
    else if(strcmp(p->nodeType,"assign_expr")==0){
        if(p->value>258){
            switch(p->value){
            case 278: printf("%s +=\n", p->nodeType); break;
			case 279: printf("%s -=\n", p->nodeType); break;
			case 280: printf("%s *=\n", p->nodeType); break;
			case 281: printf("%s /=\n", p->nodeType); break;    
            case 282: printf("%s %%=\n",p->nodeType);break;//输出%用%% 
            }
        }
        else
        {
            printf("%s\n",p->nodeType);
        }
        showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
		showAst((past)p->str, nest + 1);
    }
    else if(strcmp(p->nodeType,"cmp_expr")==0){
        switch(p->value){
        case 272: printf("%s <\n", p->nodeType); break;
		case 273: printf("%s >\n", p->nodeType); break;
		case 274: printf("%s ==\n", p->nodeType); break;
		case 275: printf("%s <=\n", p->nodeType); break;
		case 276: printf("%s >=\n", p->nodeType); break;
		case 277: printf("%s !=\n", p->nodeType); break;    
        }
        showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
		showAst((past)p->str, nest + 1); 
    }
    else if(strcmp(p->nodeType,"expr")==0){
    printf("%s %c\n", p->nodeType, (char)p->value);
	showAst(p->l, nest + 1);
	showAst(p->r, nest + 1);   
    }
	else if(strcmp(p->nodeType, "primary_expr") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "expr_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "id_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest);
			t = t->next;
		}
	}
}

