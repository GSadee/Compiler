#include "global.h"
#include "symbolTablePrinter.h"

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
	initCodeGenerator();
	yyparse();
	
	fclose(yyin);

	printSymbols();
	printOutput();

	return 1;
}