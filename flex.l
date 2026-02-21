%{
    #include <stdlib.h>
    #include "bison.tab.h"

    double *doubledup(double d) {
        double *new_double = malloc(sizeof(double));
        *new_double = d;
        return new_double;
    }
%}

%option noyywrap
%option yylineno

DIGITS        [0-9]
ESCAPABLE     ("\""|"\\"|"/"|"b"|"f"|"n"|"r"|"t"|"u"[0-9a-fA-F]{4})

%%

"{"                                                 { return CURLY_BRACKET_OPEN; }
"}"                                                 { return CURLY_BRACKET_CLOSE; }
":"                                                 { return COLUMN; }
","                                                 { return COMMA; }
"["                                                 { return SQUARE_BRACKET_OPEN; }
"]"                                                 { return SQUARE_BRACKET_CLOSE; }
"true"                                              { return TRUE; }
"false"                                             { return FALSE; }
"null"                                              { return NULLVALUE; }
((-?){DIGITS}+(\.{DIGITS}+)*((e|E)(-?){DIGITS}+)*)  { yylval.number = doubledup(atof(yytext)); return NUMBER; } /* Allocate memory for the double */
(\"([^"\\]|\\{ESCAPABLE})*\")                       { yylval.chararr = strndup(yytext + sizeof(char)*1 , strlen(yytext) - sizeof(char)*2); return STRING; } /* strndup removes sorrounding quotes from string */
<<EOF>>                                             { return END_OF_FILE; }

[ \n\t\r]                                           /* Ignore */

(.)                                                 { yyerror("Invalid symbol"); }
