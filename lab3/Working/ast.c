#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int lineno;

past newAstNode(){
	past p = (past)malloc(sizeof(ast));
	if (p == NULL){
		printf("New astNode failed.\n");
		exit(0);
	}
	memset(p, 0, sizeof(ast));
	p->line = lineno;
	p->l = NULL;
	p->r = NULL;
	p->next = NULL;
	return p;
}

//针对左递归文法
past newList(past list, past node){
	if (list != NULL){
		list->r->next = node;	//将新结点添加到链尾
		list->r = node;			//尾指针移动到链尾
		list->value += 1;
		return list;
	}
	//创建一个list
	list = newAstNode();
	list->next = node;
	list->value = 1;
	list->l = node;
	list->r = node;
	return list;
}

past newID(char *sval){
	past p = newAstNode();
	p->nodeType = "ID";
	p->str = sval;
	return p;
}

past newNUMBER(int ival){
	past p = newAstNode();
	p->nodeType = "NUMBER";
	p->value = ival;
	return p;
}

past newSTRING(char *sval){
	past p = newAstNode();
	p->nodeType = "STRING";
	p->str = sval;
	return p;
}

past newExpr(past l, int oper, past r){
	past p = newAstNode();
	p->nodeType = "expr";
	p->value = oper;
	p->l = l;
	p->r = r;
	return p;
}

past program(past l, past r){
	past list = newList(l, r);
	list->nodeType = "program";
	return list;
}

past ext_decl(past l){
	past p = newAstNode();
	p->l = l;
	p->nodeType = "ext_decl";
	return p;
}

past func_def(past type, past declr, past com_stmt){
	past p = newAstNode();
	p->nodeType = "func_def";
	p->l = type;
	p->r = declr;
	p->str = (char *)com_stmt; //如果用next保存结点，输出语法树时不同的extDecl会混合
	return p;
}

past decln(past l, past r){
	past p = newAstNode();
	p->nodeType = "decln";
	p->l = l;
	p->r = r;
	return p;
}

past init_declr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "init_declr_list";
	return list;
}

past declr(past l, past r){
	past p = newAstNode();
	p->nodeType = "declr";
	p->l = l;
	p->r = r;
	return p;
}

past intstr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "intstr_list";
	return list;
}

past dir_declr(past l, past r){
	past p = newAstNode();
	p->nodeType = "dir_declr";
	p->l = l;
	p->r = r;
	return p;
}

past para_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "para_list";
	return list;
}

past parameter(past l, past r){
	past p = newAstNode();
	p->nodeType = "parameter";
	p->l = l;
	p->r = r;
	return p;
}

past type(char *t){
	past p = newAstNode();
	p->nodeType = "type";
	p->str = t;
	return p;
}

past com_stmt(past l, past r, past s){
	past p = newAstNode();
	p->nodeType = "com_stmt";
	p->l = l;
	p->r = r;
	p->str = (char *)s; //如果用next保存结点，输出语法树时不同的extDecl会混合
	return p;
}

past begin_scope(){
	past p = newAstNode();
	p->nodeType = "begin_scope";
	return p;
}

past end_scope(){
	past p = newAstNode();
	p->nodeType = "end_scope";
	return p;
}

past stmt_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "stmt_list";
	return list;
}

past expr_stmt(past l){
	past p = newAstNode();
	p->l = l;
	p->nodeType = "expr_stmt";
	return p;
}

past if_stmt(past cond, past trueStmt, past falseStmt){
	past p = newAstNode();
	p->nodeType = "if_stmt";
	p->l = trueStmt;
	p->r = falseStmt;
	p->str = (char *)cond;
	return p;
}

past while_stmt(past cond, past stmt){
	past p = newAstNode();
	p->nodeType = "while_stmt";
	p->l = cond;
	p->r = stmt;
	return p;
}

past return_stmt(past expr){
	past p = newAstNode();
	p->nodeType = "return_stmt";
	p->l = expr;
	return p;
}

past print_stmt(past expr_list){
	past p = newAstNode();
	p->nodeType = "print_stmt";
	p->l = expr_list;
	return p;
}

past scan_stmt(past id_list){
	past p = newAstNode();
	p->nodeType = "scan_stmt";
	p->l = id_list;
	return p;
}

past assign_expr(int ival, past l, past r, past s){
	past p = newAstNode();
	p->nodeType = "assign_expr";
	p->value = ival;
	p->l = l;
	p->r = r;
	p->str = (char *)s;
	return p;
}

past cmp_expr(past l, int ival, past r){
	past p = newAstNode();
	p->nodeType = "cmp_expr";
	p->value = ival;
	p->l = l;
	p->r = r;
	return p;
}

past primary_expr(past l, past r){
	past p = newAstNode();
	p->nodeType = "primary_expr";
	p->l = l;
	p->r = r;
	return p;
}

past expr_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "expr_list";
	return list;
}

past id_list(past l, past r){
	past list = newList(l, r);
	list->nodeType = "id_list";
	return list;
}

void showAst(past p, int nest)
{
	if(p == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i++)
		printf("  ");

	if(strcmp(p->nodeType, "ID") == 0)
		printf("id: %s\n", p->str);
	else if(strcmp(p->nodeType, "NUMBER") == 0)
		printf("num: %d\n", p->value);
	else if(strcmp(p->nodeType, "STRING") == 0)
		printf("str: %s\n", p->str);
	else if(strcmp(p->nodeType, "program") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest + 1);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "ext_decl") == 0){
		printf("ext_decl \n");
		showAst(p->l, nest + 1);
	}
	else if(strcmp(p->nodeType, "func_def") == 0){
		printf("func_def \n");
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
		showAst((past)p->str, nest + 1);
	}
	else if(strcmp(p->nodeType, "decln") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "init_declr_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest + 1);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "declr") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "intstr_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest + 1);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "dir_declr") == 0){
		showAst(p->l, 0);
		showAst(p->r, nest);
	}
	else if(strcmp(p->nodeType, "para_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "parameter") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "type") == 0){
		printf("%s: %s\n", p->nodeType, p->str);
	}
	else if(strcmp(p->nodeType, "com_stmt") == 0){
		printf("compound_statement \n");
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
		showAst((past)p->str, nest + 1);
	}
	else if(strcmp(p->nodeType, "begin_scope") == 0){
		printf("\n");
	}
	else if(strcmp(p->nodeType, "end_scope") == 0){
		printf("\n");
	}
	else if(strcmp(p->nodeType, "stmt_list") == 0){
		printf("%s \n", p->nodeType);
		past t = p->l;
		int i = 1;
		for (; i <= p->value; i++){
			showAst(t, nest);
			t = t->next;
		}
	}
	else if(strcmp(p->nodeType, "expr_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
	}
	else if(strcmp(p->nodeType, "if_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst((past)p->str, nest + 1);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "while_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
	}
	else if(strcmp(p->nodeType, "return_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
	}
	else if(strcmp(p->nodeType, "print_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
	}
	else if(strcmp(p->nodeType, "scan_stmt") == 0){
		printf("%s \n", p->nodeType);
		showAst(p->l, nest + 1);
	}
	else if(strcmp(p->nodeType, "assign_expr") == 0){
		if (p->value > 258){
			switch (p->value) {
				case 278: printf("%s +=\n", p->nodeType); break;
				case 279: printf("%s -=\n", p->nodeType); break;
				case 280: printf("%s *=\n", p->nodeType); break;
				case 281: printf("%s /=\n", p->nodeType); break;
				case 282: printf("%s %%=\n", p->nodeType); break;//输出'%'用  %%
			}
		}
		else
			printf("%s \n", p->nodeType);

		showAst(p->l, nest + 1);
		showAst(p->r, nest + 1);
		showAst((past)p->str, nest + 1);
	}
	else if(strcmp(p->nodeType, "cmp_expr") == 0){
		switch (p->value) {
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
	else if(strcmp(p->nodeType, "expr") == 0){
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


