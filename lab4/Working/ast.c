
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

int lineno = 0;


void showAst(past node, int nest)
{
    if(node == NULL)
        return;
    int i = 0;
    for(i = 0; i < nest; i ++)
        printf("   ");
    if(strcmp(node->nodeType, "intValue") == 0)
        printf("%s %d\n", node->nodeType, node->ivalue);
    if(strcmp(node->nodeType, "ID") == 0)
        printf("%s %s\n", node->nodeType, node->svalue);
    else if(strcmp(node->nodeType, "expr") == 0)
        printf("%s %s %c\n",node->nodeType, node->svalue, (char)node->ivalue);
    else if(strcmp(node->nodeType, "stmt") == 0)
        printf("%s %s \n",node->nodeType, node->svalue);
    showAst(node->left, nest+1);
    showAst(node->middle, nest+1);
    showAst(node->right, nest+1);
}

past newAstNode()
{
    past node = malloc(sizeof(ast));
    if(node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    node->lineno = lineno;
    return node;
}

past newList(past list, past node)
{
    if(list != NULL)
    {
        //add node to the tail of list
        listTail(list)->next = node;

        //make tail to point to the last element of list
        listTail(list) = node;
        list->ivalue += 1;
        return list;
    }

    list = newAstNode();
    list->nodeType = "list";
    list->next = node;
    list->ivalue = 1;

    listHead(list) = node;
    listTail(list) = node;

    return list;
}



past newNum(int value)
{
    past var = newAstNode();
    var->nodeType = "intValue";
    var->ivalue = value;
    return var;
}

past newID(char* id)
{
    past var = newAstNode();
    var->nodeType = "ID";
    strcpy(var->svalue,id);
    return var;
}

past newExpr(int ivalue, char* svalue, past left, past middle, past right)
{
    past var = newAstNode();
    var->nodeType = "expr";
    var->ivalue = ivalue;
    strcpy(var->svalue,svalue);
    var->left = left;
    var->middle = middle;
    var->right = right;
    return var;
}

past newStmt(char* svalue, past left, past middle, past right)
{
    past var = newAstNode();
    var->nodeType = "stmt";
    strcpy(var->svalue,svalue);
    var->left = left;
    var->middle = middle;
    var->right = right;
    return var;
}

