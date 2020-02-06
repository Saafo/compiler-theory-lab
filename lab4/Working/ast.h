#ifndef AST_H
#define AST_H


typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
    int ivalue;
    char svalue[50];
    char* nodeType;
    past left;
    past middle;
    past right;
    past next;
    int lineno;
};

past newList(past list, past node);
#define listHead(node) node->left
#define listTail(node) node->right

void showAst(past node, int nest);
past newAstNode();
past newNum(int value);
past newID(char* id);
past newExpr(int ivalue, char* svalue, past left, past middle, past right);
past newStmt(char* svalue, past left, past middle, past right);

#endif