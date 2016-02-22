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
	int returnOffset;
	int returnType;
	vector<int> arguments;
};

extern int offset;
extern int localOffset;
extern int referenceOffset;
extern int currentScope;
extern int labelCounter;
extern int temporaryVariableCounter;
extern int currentSubProgram;
extern int currentSubProgramType;
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

int calculateOffset(int type);
int calculateReferenceOffset();

void addUntypedToken(int token);
void updateUntypedTokens(int type);

string convertIntToString(int number);
string convertRealToString(double number);
void changeScope(int scope);
void setLocalScope(int type);
