%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsonlib.h"

json_object_t *main_json;

#define YYMAXDEPTH 1000000
extern int yylex_destroy(void);
extern int yylineno;
extern FILE *yyin;
extern int yylex(void);
void yyerror(const char *);
%}

%define parse.error detailed
%define parse.trace

%code provides {
    struct json_object* flexbison(FILE *bison_input);
    void yyerror(const char *);
}

%union {
    char *chararr;
    double *number;
    struct json_object *json_object;
    struct json_array *json_array;
    struct json_pair *json_pair;
    struct json_value *json_value;
}

%start begin

%token CURLY_BRACKET_OPEN CURLY_BRACKET_CLOSE SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
%token COLUMN COMMA
%token TRUE FALSE
%token NULLVALUE
%token <number> NUMBER
%token <chararr> STRING
%token END_OF_FILE

%type <json_object> object
%type <json_array> array
%type <json_pair> pair
%type <json_pair> pairs
%type <json_pair> more_pairs
%type <json_value> value
%type <json_value> values
%type <json_value> more_values

%%

begin:
    object END_OF_FILE
    {
        main_json = $1;
        YYACCEPT;
    }
    ;

object:
    CURLY_BRACKET_OPEN pairs CURLY_BRACKET_CLOSE
    {
        $$ = json_create_object();
        $$ -> pair_list_head = $2;
    }
    ;

pairs:
    pair more_pairs
    {
        json_concatenate_pairs($1, $2);
        $$ = $1;
    }
    | %empty
    {
        $$ = NULL;
    }
    ;

more_pairs:
    COMMA pair more_pairs
    {
        json_concatenate_pairs($2, $3);
        $$ = $2;
    }
    | %empty
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

array:
    SQUARE_BRACKET_OPEN values SQUARE_BRACKET_CLOSE
    {
        $$ = json_create_array();
        $$ -> value_list_head = $2;
    }
    ;

values:
    value more_values
    {
        json_concatenate_values($1, $2);
        $$ = $1;
    }
    | %empty
    {
        $$ = NULL;
    }

more_values:
    COMMA value more_values
    {
        json_concatenate_values($2, $3);
        $$ = $2;
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
        $$ = json_create_value(JNUMBER, $1);
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
}


json_object_t* flexbison(FILE *bison_input) {

    yyin = bison_input;

    main_json = NULL;

    /* Parse input file */
    bool error = yyparse();

    json_free_temp(error);

    yylex_destroy();

    return main_json;
}
