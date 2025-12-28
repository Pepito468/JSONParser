%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsonlib.h"

json_pair_list_node_t *main_json = NULL;

extern int yylineno;
int current_line = 0;
extern FILE *yyin;
int yylex(void);
void yyerror(const char *);
%}

%define parse.error detailed
%define parse.trace

%code provides {
    struct json_pair_list_node* flexbison(FILE *bison_input);
}

%union {
    char *chararr;
    double number;
    struct json_value *json_value;
    struct json_value_list_node *json_value_list;
    struct json_pair *json_pair;
    struct json_pair_list_node *json_pair_list;
}

%start begin

%token CURLY_BRACKET_OPEN CURLY_BRACKET_CLOSE SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
%token COLUMN COMMA
%token TRUE FALSE
%token NULLVALUE
%token <number> NUMBER
%token <chararr> STRING
%token END_OF_FILE

%type <json_pair_list> object
%type <json_value_list> array
%type <json_pair> pair
%type <json_pair_list> more_pairs
%type <json_value> value
%type <json_value_list> more_values

%%

begin:
    object END_OF_FILE
    {
        main_json = $1;
        YYACCEPT;
    }
    ;

object:
    CURLY_BRACKET_OPEN pair more_pairs CURLY_BRACKET_CLOSE
    {
        $$ = json_add_pair_to_head($3, $2);
    }
    | CURLY_BRACKET_OPEN CURLY_BRACKET_CLOSE
    {
        $$ = NULL;
    }
    ;

pair:
    STRING COLUMN value
    {
        $$ = json_create_pair($1, $3);
    }
    ;

more_pairs:
    COMMA pair more_pairs
    {
        $$ = json_add_pair_to_head($3, $2);
    }
    | %empty
    {
        $$ = NULL;
    }
    ;

array:
    SQUARE_BRACKET_OPEN value more_values SQUARE_BRACKET_CLOSE
    {
        $$ = json_add_value_to_head($3, $2);
    }
    | SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
    {
        $$ = NULL;
    }
    ;

more_values:
    COMMA value more_values
    {
        $$ = json_add_value_to_head($3, $2);
    }
    | %empty
    {
        $$ = NULL;
    }
    ;

value:
    STRING
    {
        $$ = json_create_value(JSTRING, $1);
    }
    | NUMBER
    {
        $$ = json_create_value(JNUMBER, &($1));
    }
    | object
    {
        $$ = json_create_value(JOBJECT, $1);
    }
    | array
    {
        $$ = json_create_value(JARRAY, $1);
    }
    | TRUE
    {
        $$ = json_create_value(JTRUE, NULL);
    }
    | FALSE
    {
        $$ = json_create_value(JFALSE, NULL);
    }
    | NULLVALUE
    {
        $$ = json_create_value(JNULL, NULL);
    }
    ;

%%

void yyerror(const char *msg)
{
    fprintf(stderr, "JSON is malformed (line %d):\n%s\n", yylineno - 1, msg);
    yydebug = 1;
    exit(1);
}


json_pair_list_node_t* flexbison(FILE *bison_input) {

    yyin = bison_input;

    /* Parse input file */
    yyparse();

    return main_json;
}
