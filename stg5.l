%{
	#include <stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include "y.tab.h"
	#include "codegen.h"
	int number;
	char *character;
%}

%%

[0-9]+ {number = atoi(yytext); yylval.no = makeLeafNode(number); return NUM;}
"read" {return READ;}
"write" {return WRITE;}
"if" {return IFSTMT;}
"else" {return ELSE;}
"FUN" { return FUN; }
"return" {return RETU;}
[a-zA-Z][a-z0-9A-Z]* {character = strdup(yytext); yylval.no = makeVarNode(character); return ID;}
"=" {return ASGN;}
"+" {return PLUS;}
"*" {return MUL;}
"-" {return MINUS;}
"<=" {return LESSTHANEQ;}
">=" {return MORETHANEQ;}
"<" {return LESSTHAN;}
">" {return MORETHAN;}
"==" {return EQUALS;}
"!=" {return NOTEQUALS;}
[ \t\r\n] {}
. {return *yytext;}

%%

int yywrap(){
	return 0;
}