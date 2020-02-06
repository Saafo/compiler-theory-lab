# part of the makefile

CFLAGS = -O0 -g

all: lrparser.tab.c lex.yy.c ast.c main.c genllvm.c
	clang -o lrparser.out $(CFLAGS) lrparser.tab.c lex.yy.c ast.c genllvm.c main.c


lrparser.tab.c : lrparser.y
	bison -d lrparser.y

lex.yy.c : lrlex.l
	flex lrlex.l
	


clean:
	$(RM) *.out *.o lrparser.tab.c calc3.tab.h lex.yy.c
	rm -rf *.dSYM/