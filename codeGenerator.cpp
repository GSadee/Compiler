#include "global.h"
#include "codeGenerator.h"

string output;

void initCodeGenerator()
{
	createJump("lab0");
}

void createLabel()
{
	string label = "lab" + convertIntToString(labelCounter++);
    addSymbolWithType(label, LABEL);
	output += label + ":\n";
}

void createJump(string label)
{
	output += "\tjump.i\t#" + label + "\n";
}

void createExpression(int operationType, int firstOperandId, int secondOperandId)
{
	switch (operationType) {
		case PLUS:
			generateExpression("add", firstOperandId, secondOperandId);
			break;
		default:
			break;
	}
}

void generateExpression(string operation, int & firstOperandId, int & secondOperandId)
{
	typeConversion(firstOperandId, secondOperandId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	string name = "t" + convertIntToString(temporaryVariableCounter++);
	int temporaryId = addSymbolWithType(name, firstOperand.type);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);

	output += "\tadd.i\t" 
		+ convertIntToString(firstOperand.offset) 
		+ ", " 
		+ convertIntToString(secondOperand.offset)
		+ ", " 
		+ convertIntToString(temporaryEntry.offset)
		+ "\n";
}

void typeConversion(int & firstOperandId, int & secondOperandId)
{
	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	if ((INTEGER == firstOperand.type || INTEGER_VALUE == firstOperand.type)
		&& (REAL == secondOperand.type && REAL_VALUE == secondOperand.type)) {
		generateIntToReal(firstOperandId);
	} else if ((REAL == firstOperand.type || REAL_VALUE == firstOperand.type)
		&& (INTEGER == secondOperand.type && INTEGER_VALUE == secondOperand.type)) {
		generateIntToReal(secondOperandId);
	}
}

void generateIntToReal(int & id)
{
	SymbolTableEntry entry = getSymbol(id);

	string name = "t" + convertIntToString(temporaryVariableCounter++);
	int temporaryId = addSymbolWithType(name, REAL);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
	output += "\tinttoreal.i\t" 
		+ convertIntToString(entry.offset) 
		+ ", " 
		+ convertIntToString(temporaryEntry.offset) 
		+ "\n";
	id = temporaryId;
}

void printOutput()
{
	cout << endl << endl << output << endl;
}
