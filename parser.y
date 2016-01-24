%{
	#include <iostream>
	#include <vector>
    #include "global.h"
    #include "codeGenerator.h"

    using namespace std;
    
    int yyerror(char const* s);
    void generateProcedureByExpressionList(int procedureId);

    vector<int> temporaryArguments;
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
            generateLabel(createLabel());
        }
        declarations
        subprogram_declarations
        compound_statement { enableUpdatingSymbolTable = true; }
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
        declarations compound_statement 
        { 
            changeScope(SCOPE_GLOBAL);
            enableUpdatingSymbolTable = false;
        }
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
        | IF expression { generateConditionalValueJump("je", $2, 0); }
            THEN statement
            {
                int labelId = createLabel();
                SymbolTableEntry label = getSymbol(labelId);
                createJump(label.name);
                generatePreviousLabel();
            }
            ELSE statement { generateLastLabel(); }
        | WHILE { addWhileLabel(); }
            expression { generateConditionalValueJump("je", $3, 0); }
            DO statement 
            { 
                createJump(getWhileLabel());
                generateLastLabel(); 
            }
        ;
    variable: 
        ID { $$ = $1; }
        | ID '[' expression ']'
        ;
    procedure_statement:
        ID { $$ = $1; }
        | ID '(' expression_list ')' { generateProcedureByExpressionList($1); }
        ;
    expression_list:
        expression 
        {
            temporaryArguments.push_back($1);
            $$ = $1; 
        }
        | expression_list ',' expression 
        { 
            temporaryArguments.push_back($3);
            $$ = $3; 
        }
        ;
    expression:
        simple_expression { $$ = $1; }
        | simple_expression EQUAL simple_expression { $$ = createExpression(EQUAL, $1, $3); }
        | simple_expression NOT_EQUAL simple_expression { $$ = createExpression(NOT_EQUAL, $1, $3); }
        | simple_expression GREATER simple_expression { $$ = createExpression(GREATER, $1, $3); }
        | simple_expression LOWER simple_expression { $$ = createExpression(LOWER, $1, $3); }
        | simple_expression GREATER_EQUAL simple_expression { $$ = createExpression(GREATER_EQUAL, $1, $3); }
        | simple_expression LOWER_EQUAL simple_expression { $$ = createExpression(LOWER_EQUAL, $1, $3); }
        ;
    simple_expression:
        term { $$ = $1; }
        | sign term
        | simple_expression sign term { $$ = createExpression($2, $1, $3); }
        | simple_expression OR term { $$ = createExpression(OR, $1, $3); }
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
        | INTEGER_VALUE { $$ = $1; }
        | REAL_VALUE { $$ = $1; }
        | '(' expression ')' { $$ = $2; }
        | NOT factor 
        {
            int numberId = createNumberEntry(0, INTEGER_VALUE);
            $$ = generateLogicalExpression("je", $2, numberId);
        }
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

void generateProcedureByExpressionList(int procedureId)
{
    while (0 < temporaryArguments.size()) {
        generateProcedure(procedureId, temporaryArguments.front());
        temporaryArguments.erase(temporaryArguments.begin());
    }
}
