%{
    #include <stdlib.h>
    #include "bison.tab.h"
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
((-?){DIGITS}+(\.{DIGITS}+)*((e|E)(-?){DIGITS}+)*)  { yylval.number = atof(yytext); return NUMBER; }
(\"([^"\\]|\\{ESCAPABLE})*\")                       { yylval.chararr = strdup(yytext); return STRING; }
<<EOF>>                                             { return END_OF_FILE; }

[ \n\t\r]                                           /* Ignore */

(.)                                                 { yyerror("Invalid symbol"); }
