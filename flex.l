%{
    #include <stdlib.h>
    #include "jsonlib.h"
    #include "bison.tab.h"

    double *doubledup(double d) {
        double *new_double = json_malloc(sizeof(double));
        *new_double = d;
        return new_double;
    }

    char *stringndup(char *start, size_t n) {
        size_t size = 0;
        for (size_t i = 0; i < n && start[i] != '\0'; i++)
            size++;
        char *new = json_malloc(sizeof(char) * (size + 1));
        memcpy(new, start, size);
        new[size] = '\0';
        return new;
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
(\"([^"\\]|\\{ESCAPABLE})*\")                       { yylval.chararr = stringndup(yytext + sizeof(char)*1 , strlen(yytext) - sizeof(char)*2); return STRING; } /* strndup removes sorrounding quotes from string */
<<EOF>>                                             { return END_OF_FILE; }

[ \n\t\r]                                           /* Ignore */

(.)                                                 { yyerror("Invalid symbol"); }
