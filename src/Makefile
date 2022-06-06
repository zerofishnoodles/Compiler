parser: sysy.tab.c ast.c lex.yy.c semantic.c symtable.c
	gcc sysy.tab.c ast.c lex.yy.c semantic.c symtable.c -o parser 
sysy.tab.c: sysy.y 
	bison -d sysy.y  
lex.yy.c: sysy.l 
	flex sysy.l
clean:
	rm -rf parser sysy.tab.c sysy.tab.h lex.yy.c
 