%{
	#include <stdio.h>
    #include <string.h>
    int line = 2048;
    int pass_no = 1;
    int header = 0;

    struct table
    {
        int line_no;
        int label_no;
    };
    int table_count = 0;

    struct table *tables;

    int get_label_no(char *s);
    int find_line_no(char *s);
%}

%%

(L[0-9]+:[\r\n]) {
    if(pass_no == 1)
    { 
        tables[table_count].line_no = line;
        tables[table_count].label_no = get_label_no(strdup(yytext)); 
        table_count++;
    }
}
(L[0-9]) { 
    if(pass_no == 2) 
    { 
        printf("%d", find_line_no(strdup(yytext)));
    }
}
[\r\n] { 
    if( header >= 8 ) 
        line+=2;
    else 
    {
        line++; header++;
    } 
    if(pass_no == 2) 
        printf("\n"); 
    }
. { 
    if(pass_no == 2) 
    { 
        printf("%s", strdup(yytext)); 
    }
}

%%

int get_label_no(char *s)
{
    int a = 0;
    for(int i = 0; i < strlen(s); i++)
    {
        if(s[i] <= '9' && s[i] >= '0')
            a = a * 10 + s[i] - 'a';
    }
    return a;
}

int find_line_no(char *s)
{
    int a = get_label_no(s);
    for(int i = 0; i < table_count; i++)
        if(tables[i].label_no == a)
            return tables[i].line_no;
    return -1;
}

int yywrap()
{
	if(pass_no == 1)
    {
        yyin = fopen("a.txt","r");
        pass_no++;
        line = 2048;
        header = 0;
        return 0;
    }
    else
        return 1;
}

int main()
{
    tables = (struct table *)malloc(30 * sizeof(struct table));
  	yyin = fopen("a.txt","r");
  	yylex();
  	return 0;
}