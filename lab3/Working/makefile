scan.out: lex.yy.c y.tab.c ast.c main.c
	clang -g -o scan.out lex.yy.c y.tab.c ast.c main.c
	./scan.out test.c > result.txt
lex.yy.c: y.tab.h lrlex.l
	flex lrlex.l
y.tab.h y.tab.c: lrparser.y
	bison -vdty lrparser.y
clean:
	rm y.tab.* scan.out lex.yy.c y.output
	rm -rf scan.out.dSYM/