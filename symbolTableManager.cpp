#include "global.h"

int programOffset = 0;
int currentScope = SCOPE_GLOBAL;
vector<SymbolEntry> symbolTable;
vector<int> untypedTokens;

int addSymbol(string name) 
{
	SymbolEntry entry;
	entry.name = name;
	entry.scope = SCOPE_GLOBAL;
	entry.type = NONE;
	entry.offset = -1;
	symbolTable.push_back(entry);

	return symbolTable.size() - 1;
}

void updateSymbol(int id, int type)
{
	symbolTable.at(id).type = type;
	symbolTable.at(id).offset = calculateOffset(type);
}

int calculateOffset(int type)
{
	int returnValue;

	if (INTEGER == type) {
		returnValue = programOffset;
		programOffset += 4;

		return returnValue;
	}
	if (REAL == type)
	{
		returnValue = programOffset;
		programOffset += 8;

		return returnValue;
	}
}

void printSymbols()
{
	cout << "; " << "Symbol table dump" << endl;
	for (int iterator = 0; iterator < symbolTable.size(); iterator++)
	{
		SymbolEntry entry = symbolTable[iterator];
		if (SCOPE_GLOBAL == entry.scope) {
			cout << "; " << "Global ";
		} else {
			cout << "; " << "Local ";
		}
		cout << entry.name<<" ";
		switch (entry.type) {
			case INTEGER:
				cout << "integer ";
				break;
			case REAL:
				cout << "real ";
				break;
			default:
				break;
		}
		if (-1 < entry.offset) {
			cout << "offset=" << entry.offset;
		}
		cout << endl;
	}
}

void addUntypedToken(int token) 
{
	untypedTokens.push_back(token);
}

void updateUntypedTokens(int type)
{
	while(0 < untypedTokens.size())
	{
		updateSymbol(untypedTokens.front(), type);
		untypedTokens.erase(untypedTokens.begin());
	}
}
