#include "global.h"

int calculateOffset(int id)
{
	SymbolTableEntry entry = getSymbol(id);

	int returnOffset = -1;
	int variableOffset = -1;

	switch (entry.type) {
		case INTEGER:
		case REAL:
			variableOffset = getVariableOffset(entry.type);
			break;
		case ARRAY:
			variableOffset = (entry.endIndex - entry.startIndex + 1) * getVariableOffset(entry.returnType);
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

int getVariableOffset(int type)
{
	if (INTEGER == type) {
		return 4;
	}

	if (REAL == type) {
		return 8;
	}

	return -1;
}
