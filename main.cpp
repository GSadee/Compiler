#include "global.h"
#include "printer.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		cout << "Please, provide name of file." << endl;

		return 0;
	}

	extern FILE* yyin;

	yyin = fopen(argv[1], "rt+");
	
	initSymbolTable();
	yyparse();
	
	fclose(yyin);

	printSymbols();

	return 1;
}