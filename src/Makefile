parser: sysy.l sysy.y ast.c
	bison -d sysy.y -o parser.tab.c  --debug
	flex sysy.l
	gcc -o $@ parser.tab.c lex.yy.c ast.c
clean: 
	rm -rf parser.tab.c lex.yy.c parser parser.tab.h
