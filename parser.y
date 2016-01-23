%{
	#include <iostream>
	#include <vector>
    #include "global.h"

    using namespace std;
    
    int yyerror(char const* s);
%}

%error-verbose

%token PROGRAM
%token ID
%token INTEGER
%token INTEGER_VALUE
%token REAL
%token REAL_VALUE
%token VAR
%token START
%token END
%token ARRAY
%token DOTS
%token OF
%token FUNCTION
%token PROCEDURE
%token ASSIGNMENT_OPERATOR
%token MULTIPLICATION
%token DIVISION
%token MODULO
%token IF
%token ELSE
%token THEN
%token WHILE
%token DO
%token EQUAL
%token NOT_EQUAL
%token GREATER
%token LOWER
%token GREATER_EQUAL
%token LOWER_EQUAL
%token AND
%token OR
%token NOT
%token PLUS
%token MINUS
%token NONE
%token DONE 0

%%
    program: 
        PROGRAM ID '(' identifier_program_list ')' ';'
        declarations
        subprogram_declarations
        compound_statement
        '.'
    	;
    identifier_program_list: 
        ID
        | identifier_program_list ',' ID 
        ;
    declarations: 
        declarations VAR identifier_list':' type';' {
            updateUntypedTokens($5);
        }
        |
        ;
    identifier_list:
        ID { addUntypedToken($1); }
        | identifier_list ',' ID { addUntypedToken($3); }
        ;
    type: 
        standard_type { $$ = $1; }
        | ARRAY '[' INTEGER_VALUE DOTS INTEGER_VALUE ']' OF standard_type
        ;
    standard_type: 
        INTEGER { $$ = INTEGER; }
        | REAL { $$ = REAL; }
        ;
    subprogram_declarations: 
        subprogram_declarations subprogram_declaration ';'
        |
        ; 
    subprogram_declaration: 
        subprogram_head declarations compound_statement 
        ;
    subprogram_head: 
        FUNCTION ID arguments ':' standard_type ';'
        | PROCEDURE ID arguments ';'
        ;
    arguments:
        '(' parameter_list ')' 
        | 
        ;
    parameter_list:
        identifier_list ':' type
        | parameter_list ';' identifier_list ':' type
        ;
    compound_statement:
        START optional_statements END
        ;
    optional_statements: 
        statement_list 
        |
        ; 
    statement_list: 
        statement 
        | statement_list ';' statement
        ;
    statement: 
        variable ASSIGNMENT_OPERATOR expression
        | procedure_statement
        | compound_statement
        | IF expression THEN statement ELSE statement
        | WHILE expression DO statement
        ;
    variable: 
        ID 
        | ID '[' expression ']' 
        ;
    procedure_statement:
        ID
        | ID '(' expression_list ')'
        ;
    expression_list:
        expression 
        | expression_list ',' expression
        ;
    expression:
        simple_expression 
        | simple_expression EQUAL simple_expression
        | simple_expression NOT_EQUAL simple_expression
        | simple_expression GREATER simple_expression
        | simple_expression LOWER simple_expression
        | simple_expression GREATER_EQUAL simple_expression
        | simple_expression LOWER_EQUAL simple_expression
        ;
    simple_expression:
        term
        | sign term
        | simple_expression sign term
        | simple_expression OR term
        ;
    term:
        factor 
        | term MULTIPLICATION factor
        | term DIVISION factor
        | term MODULO factor
        | term AND factor
        ;
    factor:
        variable
        | ID '(' expression_list ')'
        | INTEGER_VALUE
        | REAL_VALUE
        | '(' expression ')'
        | NOT factor
        ;
    sign: 
        PLUS { $$ = $1; }
        | MINUS
        ;
%%

int yyerror(char const* s)
{
    printf("%s\n", s);

    return 1;
}
