#include "global.h"

using namespace std;

void typeConversion(int & firstOperandId, int & secondOperandId)
{
	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	if ((INTEGER == firstOperand.type || INTEGER_VALUE == firstOperand.type)
		&& (REAL == secondOperand.type || REAL_VALUE == secondOperand.type)) {
		generateIntToReal(firstOperandId);
	} else if ((REAL == firstOperand.type || REAL_VALUE == firstOperand.type)
		&& (INTEGER == secondOperand.type || INTEGER_VALUE == secondOperand.type)) {
		generateIntToReal(secondOperandId);
	}
}

void typeConversionToFirstOperand(int & firstOperandId, int & secondOperandId)
{
	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	if ((INTEGER == firstOperand.type || INTEGER_VALUE == firstOperand.type)
		&& (REAL == secondOperand.type || REAL_VALUE == secondOperand.type)) {
		generateRealToInt(secondOperandId);
	} else if ((REAL == firstOperand.type || REAL_VALUE == firstOperand.type)
		&& (INTEGER == secondOperand.type || INTEGER_VALUE == secondOperand.type)) {
		generateIntToReal(secondOperandId);
	} else if (FUNCTION == secondOperand.type) {
		if ((INTEGER == firstOperand.type || INTEGER_VALUE == firstOperand.type)
			&& (REAL == secondOperand.returnType || REAL_VALUE == secondOperand.returnType)) {
			generateRealToInt(secondOperandId);
		} else if ((REAL == firstOperand.type || REAL_VALUE == firstOperand.type)
			&& (INTEGER == secondOperand.returnType || INTEGER_VALUE == secondOperand.returnType)) {
			generateIntToReal(secondOperandId);
		}
	}
}

string convertIntegerToString(int number)
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
