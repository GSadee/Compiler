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

int createExpression(int operationType, int firstOperandId, int secondOperandId)
{
	switch (operationType) {
		case PLUS:
			return generateExpression("add", firstOperandId, secondOperandId);
		case MULTIPLICATION:
			return generateExpression("mul", firstOperandId, secondOperandId);
		case ASSIGNMENT_OPERATOR:
			return generateAssignmentOperation(firstOperandId, secondOperandId);
		default:
			break;
	}

	return -1;
}

int generateExpression(string operation, int & firstOperandId, int & secondOperandId)
{
	typeConversion(firstOperandId, secondOperandId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	string name = "$t" + convertIntToString(temporaryVariableCounter++);
	int temporaryId = addSymbolWithType(name, firstOperand.type);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);

	output += "\t" + operation + getSuffix(temporaryEntry.type) + "\t" 
		+ convertIntToString(firstOperand.offset) 
		+ ", " 
		+ convertIntToString(secondOperand.offset)
		+ ", " 
		+ convertIntToString(temporaryEntry.offset)
		+ "\n";

	return temporaryId;
}

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

void generateIntToReal(int & id)
{
	SymbolTableEntry entry = getSymbol(id);

	string name = "$t" + convertIntToString(temporaryVariableCounter++);
	int temporaryId = addSymbolWithType(name, REAL);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
	output += "\tinttoreal.i\t" 
		+ convertIntToString(entry.offset) 
		+ ", " 
		+ convertIntToString(temporaryEntry.offset) 
		+ "\n";
	id = temporaryId;
}

string getSuffix(int type)
{
	if (INTEGER == type || INTEGER_VALUE == type) {
		return ".i";
	}

	if (REAL == type || REAL_VALUE == type) {
		return ".r";
	}

	return "";
}

int generateAssignmentOperation(int firstOperandId, int secondOperandId)
{
	typeConversion(firstOperandId, secondOperandId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	output += "\tmov" + getSuffix(firstOperand.type) + "\t" 
		+ convertIntToString(secondOperand.offset) 
		+ ", " 
		+ convertIntToString(firstOperand.offset)
		+ "\n";

	return -1;
}

void generateExit()
{
	output += "\texit\n";
}

void generateProcedure(int procedureId, int argumentId)
{
	SymbolTableEntry procedure = getSymbol(procedureId);
	SymbolTableEntry argument = getSymbol(argumentId);

	output += "\t" + procedure.name + getSuffix(argument.type)
		+ "\t" + convertIntToString(argument.offset) 
		+ "\n";
}

void printOutput()
{
	cout << endl << endl << output << endl;
}
