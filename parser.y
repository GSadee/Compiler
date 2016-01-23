%{
	#include <iostream>
	#include <vector>
    #include "global.h"
    #include "codeGenerator.h"

    using namespace std;
    
    int yyerror(char const* s);
%}

%error-verbose

%token PROGRAM
%token PROGRAM_ID
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
%token LABEL
%token DONE 0

%%
    program: 
        PROGRAM ID '(' identifier_program_list ')' ';' 
        { 
            removeSymbol($2);
            createLabel();
        }
        declarations
        subprogram_declarations
        compound_statement
        '.' { generateExit(); }
    	;
    identifier_program_list: 
        ID { removeSymbol($1); }
        | identifier_program_list ',' ID { removeSymbol($3); }
        ;
    declarations: 
        declarations VAR identifier_list ':' type ';' {
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
        subprogram_head { changeScope(SCOPE_LOCAL); }
        declarations compound_statement { changeScope(SCOPE_GLOBAL); }
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
        variable ASSIGNMENT_OPERATOR expression { $$ = createExpression(ASSIGNMENT_OPERATOR, $1, $3); }
        | procedure_statement
        | compound_statement
        | IF expression THEN statement ELSE statement
        | WHILE expression DO statement
        ;
    variable: 
        ID { $$ = $1; }
        | ID '[' expression ']'
        ;
    procedure_statement:
        ID { $$ = $1; }
        | ID '(' expression_list ')' { generateProcedure($1, $3); }
        ;
    expression_list:
        expression { $$ = $1; }
        | expression_list ',' expression
        ;
    expression:
        simple_expression { $$ = $1; }
        | simple_expression EQUAL simple_expression
        | simple_expression NOT_EQUAL simple_expression
        | simple_expression GREATER simple_expression
        | simple_expression LOWER simple_expression
        | simple_expression GREATER_EQUAL simple_expression
        | simple_expression LOWER_EQUAL simple_expression
        ;
    simple_expression:
        term { $$ = $1; }
        | sign term
        | simple_expression sign term { $$ = createExpression($2, $1, $3); }
        | simple_expression OR term
        ;
    term:
        factor { $$ = $1; }
        | term MULTIPLICATION factor { $$ = createExpression(MULTIPLICATION, $1, $3); }
        | term DIVISION factor { $$ = createExpression(DIVISION, $1, $3); }
        | term MODULO factor { $$ = createExpression(MODULO, $1, $3); }
        | term AND factor { $$ = createExpression(AND, $1, $3); }
        ;
    factor:
        variable { $$ = $1; }
        | ID '(' expression_list ')' { $$ = $2; }
        | INTEGER_VALUE
        | REAL_VALUE
        | '(' expression ')' { $$ = $2; }
        | NOT factor { $$ = $2; }
        ;
    sign: 
        PLUS { $$ = PLUS; }
        | MINUS { $$ = MINUS; }
        ;
%%

int yyerror(char const* s)
{
    printf("%s at line %d\n", s, lineno);

    return 1;
}
