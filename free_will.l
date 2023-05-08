%{
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "structures/tree.h"
#include "structures/hashmap.h"
#include "structures/matrix.h"

#include "free_will.tab.h"

int yylex(void);
int yyerror(char*);
%}

identifier	[a-zA-Z][a-zA-Z0-9]*
num		[0-9]+

whitespace	[ \t]+
commentA	(\{.*\})
commentB	(\(\*.*\*\))
%%

{commentA}	{ ; }
{commentB}	{ ; }

"print"		{ fprintf( stderr, "[PRINT]" ); return PRINT; }
"scan"		{ fprintf( stderr, "[SCAN]" ); return SCAN; }
"var"		{ fprintf( stderr, "[VAR]" ); return VAR; }

"div"		{ fprintf( stderr, "[MULOP:%s]", yytext ); yylval.opval = DIV; return MULOP; }
"mod"		{ fprintf( stderr, "[MULOP:%s]", yytext ); yylval.opval = MOD; return MULOP; }
"*"		{ fprintf( stderr, "[MULOP:%s]", yytext ); yylval.opval = STAR; return MULOP; }
"/"		{ fprintf( stderr, "[MULOP:%s]", yytext ); yylval.opval = SLASH; return MULOP; }
"+"		{ fprintf( stderr, "[ADDOP:%s]", yytext ); yylval.opval = PLUS; return ADDOP; }
"-"		{ fprintf( stderr, "[ADDOP:%s]", yytext ); yylval.opval = MINUS; return ADDOP; }

"("		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
")"		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
";"		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
","		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"."		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"["		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
"]"		{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
":="		{ fprintf( stderr, "[ASSIGNOP]" ); return ASSIGNOP; }

\n		{ fprintf( stderr, "\n" ); }
{identifier}	{ fprintf( stderr, "[ID:%s]", yylval.sval = strdup(yytext) ); return ID; }
{num}		{ fprintf( stderr, "[INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }
{num}\.{num}	{ fprintf( stderr, "[RNUM:%f]", yylval.rval = atof(yytext) ); return RNUM; }
{whitespace}	{ ; }

.		{ fprintf( stderr, "<%c>", yytext[0] ); return yytext[0]; }
%%

int yyerror( char* message ) {
	fprintf( stderr, "ERROR: %s\n", message );
	exit(1);
}