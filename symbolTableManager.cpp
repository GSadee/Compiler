#include "global.h"

int offset = 0;
int localOffset = 0;
int referenceOffset = 8;
int currentScope = SCOPE_GLOBAL;
int labelCounter = 0;
int temporaryVariableCounter = 0;
int currentSubProgram = -1;
int currentSubProgramType = -1;
vector<SymbolTableEntry> symbolTable;
vector<int> untypedTokens;
vector<int> temporaryArguments;

void initSymbolTable()
{
	addSymbolWithType("write", PROCEDURE);
	addSymbolWithType("read", PROCEDURE);
}

int addSymbol(string name) 
{
	SymbolTableEntry entry;
	entry.name = name;
	entry.scope = currentScope;
	entry.reference = false;
	entry.type = NONE;
	entry.offset = -1;
	entry.returnOffset = -1;
	entry.returnType = -1;
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

SymbolTableEntry getSymbol(int id)
{
	if (symbolTable.size() > id) {
		return symbolTable.at(id);
	}

	return symbolTable.at(0);
}

int getSymbolId(string name)
{
	for (int i = 0; i < symbolTable.size(); i++) {
		if (0 == name.compare(symbolTable.at(i).name)) {
			return i;
		}
	}

	return -1;
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
	int variableOffset = -1;

	switch (type) {
		case INTEGER:
			variableOffset = 4;
			break;
		case REAL:
			variableOffset = 8;
			break;
		default:
			return -1;
	}

	if (SCOPE_GLOBAL == currentScope) {
		returnOffset = offset;
		offset += variableOffset;
	} else if (SCOPE_LOCAL == currentScope) {
		localOffset += variableOffset;
		returnOffset = localOffset;
	}

	return returnOffset;
}

int calculateReferenceOffset()
{
	int returnOffset = referenceOffset;
	referenceOffset += 4;

	return returnOffset;
}

void addUntypedToken(int token) 
{
	untypedTokens.push_back(token);
}

void updateUntypedTokens(int type)
{
	while (0 < untypedTokens.size()) {
		updateSymbolWithType(untypedTokens.front(), type);
		untypedTokens.erase(untypedTokens.begin());
	}
}

string convertIntToString(int number)
{
	string result;
	sprintf((char*) result.c_str(), "%d", number);
	
	return result.c_str();
}

string convertRealToString(double number)
{
	string result;
	sprintf((char*) result.c_str(), "%.2f", number);
	
	return result.c_str();
}

void changeScope(int scope)
{
	if (SCOPE_LOCAL == currentScope && SCOPE_GLOBAL == scope) {
		localOffset = 0;
		referenceOffset = 8;
	}

	currentScope = scope;
}

void setLocalScope(int type)
{
	if (FUNCTION == type) {
		referenceOffset += 4;
	}

	changeScope(SCOPE_LOCAL);
}

string getOffset(SymbolTableEntry entry)
{
	if (0 > entry.offset && (INTEGER_VALUE == entry.type || REAL_VALUE == entry.type)) {
		return "#" + entry.name;
	}

	if (true == entry.reference) {
		return "*BP+" + convertIntToString(entry.offset);
	}

	if (SCOPE_LOCAL == currentScope && FUNCTION == entry.type) {
		return "*BP+8";
	}

	if (SCOPE_GLOBAL == currentScope && FUNCTION == entry.type) {
		return convertIntToString(entry.returnOffset);
	}

	if (SCOPE_LOCAL == entry.scope) {
		return "BP-" + convertIntToString(entry.offset);
	}

	return convertIntToString(entry.offset);
}