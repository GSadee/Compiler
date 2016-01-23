#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include "parser.h"
#include "symbolTableManager.h"

using namespace std;

int yylex ();
int yyparse();