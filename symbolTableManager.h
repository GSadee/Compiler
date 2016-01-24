#include <vector>
#include "offsetCalculator.h"

using namespace std;

#define SCOPE_GLOBAL 0
#define SCOPE_LOCAL 1

struct SymbolTableEntry
{
	string name;
	int type;
	int scope;
	int offset;
	bool reference;
	
	int returnType;
	int returnOffset;
	vector<int> arguments;
};

extern int offset;
extern int localOffset;
extern int referenceOffset;
extern int currentScope;
extern int labelCounter;
extern int temporaryVariableCounter;
extern int currentSubProgram;
extern vector<SymbolTableEntry> symbolTable;
extern vector<int> untypedTokens;
extern vector<int> temporaryArguments;

void initSymbolTable();

int addSymbol(string name);
int addSymbolWithType(string name, int type);
void updateSymbolWithType(int id, int type);
SymbolTableEntry getSymbol(int id);
int getSymbolId(string name);
void removeSymbol(int id);

void addUntypedToken(int token);
void updateUntypedTokens(int type);

void changeScope(int scope);
void setLocalScope(int type);

string getOffset(SymbolTableEntry entry);
