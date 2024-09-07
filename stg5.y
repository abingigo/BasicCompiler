%{
	#include <stdio.h>
	#include "codegen.h"
	#include "codegen.c"
	int yylex(void);
    extern int yyerror(char const *s);
%}

%union{
	struct tnode *no;
}

%type <no> program slist stmt expr NUM ID READA WRITEA

%token PLUS MINUS MUL LESSTHAN MORETHAN LESSTHANEQ MORETHANEQ EQUALS NOTEQUALS ID NUM READ WRITE ASGN IFSTMT ELSE FUN RETU

%left PLUS MINUS
%left MUL

%%

program : READA ID ASGN FUN '(' ID ')' '{' slist '}' WRITEA { 
            $$ = $1; genCode(); codeEval($1, 0); codeEval($9, 0); codeEval($11, 0); endProgram(); exit(0);
            }
     ;

slist : slist stmt {$$ = makeConnectorNode($1, $2);}
     | stmt {$$ = $1;}
     ;

stmt: ID ASGN expr ';' {$$ = makeOperatorNode('=', $1, $3);}
    | IFSTMT expr '{' slist '}' ELSE '{' slist '}' {$$ = makeIfNode($2, $4, $8);}
    | IFSTMT expr '{' slist '}' {$$ = makeIfNode($2, $4, NULL);}
    | RETU expr ';' {$$ = makeReturnNode($2);}
	;

expr : expr PLUS expr {$$ = makeOperatorNode('+', $1, $3);}
	| expr MINUS expr {$$ = makeOperatorNode('-', $1, $3);}
	| expr MUL expr {$$ = makeOperatorNode('*', $1, $3);}
    | expr MORETHANEQ expr { $$ = makeOperatorNode('>', $1, $3); }
    | expr LESSTHANEQ expr { $$ = makeOperatorNode('<', $1, $3); }
    | expr MORETHAN expr { $$ = makeOperatorNode('.', $1, $3); }
    | expr LESSTHAN expr { $$ = makeOperatorNode(',', $1, $3); }
    | expr EQUALS expr { $$ = makeOperatorNode('~', $1, $3); }
    | expr NOTEQUALS expr { $$ = makeOperatorNode('!', $1, $3); }
	| '(' expr ')'		{$$ = $2;}
	| ID {$$ = $1;}
	| NUM {$$ = $1;}
    | FUN '(' expr ')' { $$ = makeFunctionNode($3); } 
	;

READA : READ '(' ID ')' ';' {$$ = makeReadNode();};
        ;
WRITEA : WRITE '(' ID ')' ';' {$$ = makeWriteNode();}

%%

int yyerror(char const *s) { 
    printf("yyerror  %s\n",s); 
    return 0;
}

int main()
{
    lst = NULL;
    yyparse();
    return 0;
}