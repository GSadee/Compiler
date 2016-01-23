#include <vector>

using namespace std;

#define SCOPE_GLOBAL 0
#define SCOPE_LOCAL 1

struct SymbolEntry
{
	int scope;
	int type;
	bool reference;
	string name;
	int offset;
};

extern int programOffset;
extern int currentScope;
extern vector<SymbolEntry> symbolTable;
extern vector<int> untypedTokens;

int addSymbol(string name);
void updateSymbol(int id, int type);
int calculateOffset(int type);
void printSymbols();

void addUntypedToken(int token);
void updateUntypedTokens(int type);