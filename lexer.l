%option outfile = "lexer.c"
%option noyywrap

%{
	#include "global.h"

	int lineno = 0;

	bool updateSymbolTable = true;
%}

blank		[ \t\r]
whitespaces	{blank}+
newline		[\n]
letter		[A-Za-z]
digit		[0-9]
integer		{digit}+
real		{digit}+(\.{digit}+)
id 			{letter}({letter}|{digit})*

%%
{whitespaces}
{newline}		{ ++lineno; }
{integer}		{ 
					yylval = atoi(yytext); 
					return INTEGER_VALUE;
				}
{real}			{ 
					yylval = atol(yytext);
					return REAL_VALUE;
				}
"program"		{ return PROGRAM; }
"var"			{ return VAR; }
"integer"		{ return INTEGER; }
"real"			{ return REAL; }
"begin"			{ 
					updateSymbolTable = false; 
					return START; 
				}
"end"			{ 
					updateSymbolTable = true; 
					return END; 
				}
":="			{ return ASSIGNMENT_OPERATOR; }
"array"			{ return ARRAY; }
".."			{ return DOTS; }
"of"			{ return OF; }
"function"		{ return FUNCTION; }
"procedure"		{ return PROCEDURE; }
"if"			{ return IF; }
"then"			{ return THEN; }
"else"			{ return ELSE; }
"while"			{ return WHILE; }
"do"			{ return DO; }
"="				{ return EQUAL; }
"<>"			{ return NOT_EQUAL; }
">"				{ return GREATER; }
"<"				{ return LOWER; }
">="			{ return GREATER_EQUAL; }
"<="			{ return LOWER_EQUAL; }
"*"				{ return MULTIPLICATION; }
"/"				{ return DIVISION; }
"div"			{ return DIVISION; }
"mod"			{ return MODULO; }
"and"			{ return AND; }
"or"			{ return OR; }
"not"			{ return NOT; }
"+"				{ return PLUS; }
"-"				{ return MINUS; }
{id}			{ 
					if (updateSymbolTable) { 
						yylval = addSymbol(yytext); 
					}
					return ID; 
				}
.				{ return yytext[0]; }
<<EOF>>			{ return DONE; }
%%
