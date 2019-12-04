typedef struct _ast ast;
typedef struct _ast *past;

struct _ast {
	char *nodeType;
	int line;
	int value;
	char *str;
	past l;
	past r;
	past next;
};

past astHead;	//语法树的根节点

past newID(char *sval);
past newNUMBER(int ival);
past newSTRING(char *sval);
past newExpr(past l, int oper, past r);
past program(past l, past r);
past ext_decl(past l);
past func_def(past type, past declr, past com_stmt);
past decln(past l, past r);
past init_declr_list(past l, past r);
past declr(past l, past r);
past intstr_list(past l, past r);
past dir_declr(past l, past r);
past para_list(past l, past r);
past parameter(past l, past r);
past type(char *t);
past com_stmt(past l, past r, past s);
past begin_scope();
past end_scope();
past stmt_list(past l, past r);
past expr_stmt(past l);
past if_stmt(past cond, past trueStmt, past falseStmt);
past while_stmt(past cond, past stmt);
past return_stmt(past expr);
past print_stmt(past expr_list);
past scan_stmt(past id_list);
past assign_expr(int ival, past l, past r, past s);
past cmp_expr(past l, int ival, past r);
past primary_expr(past l, past r);
past expr_list(past l, past r);
past id_list(past l, past r);
void showAst(past p, int nest);

