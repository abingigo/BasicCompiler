all:
	bison --yacc -d stg5.y
	flex stg5.l
	gcc lex.yy.c y.tab.c
	./a.out > a.txt < $(input)
	flex fixlabels.l
	gcc lex.yy.c
	./a.out > B190880CS.xsm
clean:
	rm lex.yy.c y.tab.c y.tab.h a.txt a.out B190880CS.xsm