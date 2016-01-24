#include "global.h"

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