

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "genllvm.h"
char symbol[50][10];

int temVarNum = 0;



void addSymbol(char* id,int num)
{
    strcpy(symbol[num-1],id);
}

int getSymbol(char* id)
{
    for(int i=0;i<50;i++){
        if(symbol[i]!=NULL)
            if(strcmp(symbol[i],id)==0)
                return i+1;
    }
}




int getTemVarNum()
{
    temVarNum ++;
    return temVarNum;
}
void genStmt(past node)
{
    if(node == NULL)
        return;
    if(strcmp(node->svalue,"decl")==0)
        genDecl(node);
    else if(strcmp(node->svalue,"sele")==0)
        genSele(node);
    else if(strcmp(node->svalue,"assign")==0)
        genAssgin(node);
}

void genAssgin(past node)
{
    if(node == NULL)
        return;
    char id[50];
    char value[50];
    sprintf(id,"%%%d",getSymbol(node->left->svalue));
    genPrimaryExpr(node->middle,value);
    printf("  store i32 %s, i32* %s align 4\n",value,id);
}


void genSele(past node)
{
    char condition[50];

    genPrimaryExpr(node->left,condition);
    int temp1 = getTemVarNum();
    int temp2 = getTemVarNum();

    printf("  br i1 %s, label %d, label %d\n",condition,temp1,temp2);
    printf("  ;<label>:%d\n",temp1);
    genStmt(node->middle);
    printf("  br label %d\n",temp2);
    printf("  ;<label>:%d\n",temp2);
    if(node->right != NULL){
        genStmt(node->right);
    }


}

void genDecl(past node)
{
    if(node == NULL)
        return;

    char id[50];
    char value[50];
    sprintf(id,"%s",node->left->svalue);
    int tar = getTemVarNum();
    addSymbol(id,tar);
    printf("  %%%d = alloca i32, align 4\n",tar);

    if(node->middle != NULL){
        genPrimaryExpr(node->middle,value);
        printf("  store i32 %s, i32* %%%d, align 4\n",value,tar);
    }
}

void genPrimaryExpr(past node, char* operand)
{
    if(node == NULL)
        return;
    if(strcmp(node->nodeType, "intValue") == 0)
    {
        if(operand != NULL)
            sprintf(operand, "%d", node->ivalue);
    }
    else if(strcmp(node->nodeType, "expr") == 0)
    {
        //表达式的中间结果用临时变量保存
        //其结果为当前的 temVarNum
        genExpr(node);
        if(operand != NULL)
            sprintf(operand, "%%%d", temVarNum);
    }
    else if(strcmp(node->nodeType, "ID") == 0)
    {
        int order = getSymbol(node->svalue);
        if(operand != NULL){
            int temp = getTemVarNum();
            printf("  %%%d = load i32, i32* %%%d, align4\n",temp,order);
            sprintf(operand, "%%%d", temp);
        }


    }
    else
    {
        printf("ERROR: 发现不支持的运算类型, line %d\n", node->lineno);
    }
}

void genExpr(past node)
{
    if(node == NULL)
        return;

    if( strcmp(node->nodeType, "expr") == 0)
    {
        char loperand[50];
        char roperand[50];
        char oper[50];

        genPrimaryExpr(node->left, loperand);
        genPrimaryExpr(node->middle, roperand);
        if((node->ivalue == '-')&&strcmp(node->svalue,"mul") == 0)
        {
            //处理一元表达式
            sprintf(loperand, "0");
        }


        switch(node->ivalue)
        {
            case '+': sprintf(oper, "add nsw"); break;
            case '-': sprintf(oper, "sub nsw"); break;
            case '*': sprintf(oper, "mul nsw"); break;
            case '/': sprintf(oper, "sdiv"); break;
            case '%': sprintf(oper, "srem"); break;
            case 'N': sprintf(oper, "sub nsw"); break;
            case '>': sprintf(oper, "icmp sgt "); break;
            case '<': sprintf(oper, "icmp slt "); break;
        }

        printf("  %%%d = %s i32 %s, %s\n",	getTemVarNum(), oper, loperand, roperand);
    }
}
