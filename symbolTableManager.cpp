#include "global.h"

int programOffset = 0;
int currentScope = SCOPE_GLOBAL;
int labelCounter = 0;
vector<SymbolTableEntry> symbolTable;
vector<int> untypedTokens;

void initSymbolTable()
{
	addSymbolWithType("write", PROCEDURE);
	addSymbolWithType("read", PROCEDURE);
}

int addSymbol(string name) 
{
	SymbolTableEntry entry;
	entry.name = name;
	entry.scope = SCOPE_GLOBAL;
	entry.type = NONE;
	entry.offset = -1;
	symbolTable.push_back(entry);

	return symbolTable.size() - 1;
}

int addSymbolWithType(string name, int type) 
{
	int id = addSymbol(name);
	updateSymbolWithType(id, type);

	return id;
}

void updateSymbolWithType(int id, int type)
{
	symbolTable.at(id).type = type;
	symbolTable.at(id).offset = calculateOffset(type);
}

void removeSymbol(int id)
{
	if (symbolTable.size() > id) {
		symbolTable.erase(symbolTable.begin() + id);
	}
}

int calculateOffset(int type)
{
	int returnOffset = -1;

	switch (type) {
		case INTEGER:
			returnOffset = programOffset;
			programOffset += 4;
			break;
		case REAL:
			returnOffset = programOffset;
			programOffset += 8;
			break;
	}

	return returnOffset;
}

void addUntypedToken(int token) 
{
	untypedTokens.push_back(token);
}

void updateUntypedTokens(int type)
{
	while(0 < untypedTokens.size())
	{
		updateSymbolWithType(untypedTokens.front(), type);
		untypedTokens.erase(untypedTokens.begin());
	}
}

string convertIntToString(int number) {
	string result;
	sprintf((char*) result.c_str(), "%d", number);
	
	return result.c_str();
}
