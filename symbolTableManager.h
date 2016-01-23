#include <vector>

using namespace std;

#define SCOPE_GLOBAL 0
#define SCOPE_LOCAL 1

struct SymbolTableEntry
{
	int scope;
	int type;
	bool reference;
	string name;
	int offset;
};

extern int programOffset;
extern int currentScope;
extern int labelCounter;
extern vector<SymbolTableEntry> symbolTable;
extern vector<int> untypedTokens;

void initSymbolTable();
int addSymbol(string name);
int addSymbolWithType(string name, int type);
void updateSymbolWithType(int id, int type);
void removeSymbol(int id);

int calculateOffset(int type);

void addUntypedToken(int token);
void updateUntypedTokens(int type);

string convertIntToString(int number);
