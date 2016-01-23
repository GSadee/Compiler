#include "global.h"

using namespace std;

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf("%s\n", "Please, provide name of file.");
		return 0;
	}

	extern FILE* yyin;

	yyin = fopen(argv[1], "rt+");
	yyparse();
	fclose(yyin);
	printSymbols();

	return 1;
}