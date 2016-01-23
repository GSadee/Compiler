#include "global.h"
#include "codeGenerator.h"

string output;

void initCodeGenerator()
{
	createJump("lab0");
}

int createLabel()
{
	string label = "lab" + convertIntToString(labelCounter++);
    
    return addSymbolWithType(label, LABEL);
}

void generateLabel(int labelId)
{
	SymbolTableEntry label = getSymbol(labelId);
	output += label.name + ":\n";
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
		case AND:
			return generateExpression("and", firstOperandId, secondOperandId);
		case OR:
			return generateExpression("or", firstOperandId, secondOperandId);
		case ASSIGNMENT_OPERATOR:
			return generateAssignmentOperation(firstOperandId, secondOperandId);
		case EQUAL:
			return generateLogicalExpression("je", firstOperandId, secondOperandId);
		case NOT_EQUAL:
			return generateLogicalExpression("jne", firstOperandId, secondOperandId);
		case GREATER:
			return generateLogicalExpression("jg", firstOperandId, secondOperandId);
		case LOWER:
			return generateLogicalExpression("jl", firstOperandId, secondOperandId);
		case GREATER_EQUAL:
			return generateLogicalExpression("jge", firstOperandId, secondOperandId);
		case LOWER_EQUAL:
			return generateLogicalExpression("jle", firstOperandId, secondOperandId);
		default:
			break;
	}

	return -1;
}

int generateLogicalExpression(string operation, int firstOperandId, int secondOperandId)
{
	typeConversion(firstOperandId, secondOperandId);

	int labelId = createLabel();
	generateConditionalJump(operation, firstOperandId, secondOperandId, labelId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);

	int temporaryId = createTemporaryVariableEntry(firstOperand.type);
	int number0Id = createNumberEntry(0);

	generateAssignmentOperation(temporaryId, number0Id);

	int nextLabelId = createLabel();
	SymbolTableEntry nextLabel = getSymbol(nextLabelId);
	createJump(nextLabel.name);

	generateLabel(labelId);

	int number1Id = createNumberEntry(1);

	generateAssignmentOperation(temporaryId, number1Id);

	generateLabel(nextLabelId);

	return temporaryId;
}

int createTemporaryVariableEntry(int type)
{
	string name = "$t" + convertIntToString(temporaryVariableCounter++);
	
	return addSymbolWithType(name, type);
}

int createNumberEntry(int number)
{
	string name = convertIntToString(number);
	
	return addSymbolWithType(name, INTEGER_VALUE);	
}

void generateConditionalValueJump(string operation, int operandId, int number)
{
	int labelId = createLabel();
	int numberId = createNumberEntry(number);

	generateConditionalJump(operation, operandId, numberId, labelId);
}

void generateConditionalJump(string operation, int firstOperandId, int secondOperandId, int labelId)
{
	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);
	SymbolTableEntry label = getSymbol(labelId);

	output += "\t"
		+ operation + getSuffix(firstOperand.type) + "\t" 
		+ getOffset(firstOperand) 
		+ ", " 
		+ getOffset(secondOperand)
		+ ", #" 
		+ label.name
		+ "\n";
}

int generateExpression(string operation, int firstOperandId, int secondOperandId)
{
	typeConversion(firstOperandId, secondOperandId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	int temporaryId = createTemporaryVariableEntry(firstOperand.type);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);

	output += "\t" + operation + getSuffix(temporaryEntry.type) + "\t" 
		+ getOffset(firstOperand) 
		+ ", " 
		+ getOffset(secondOperand)
		+ ", " 
		+ getOffset(temporaryEntry)
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

	int temporaryId = createTemporaryVariableEntry(REAL);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
	output += "\tinttoreal.i\t" 
		+ getOffset(entry) 
		+ ", " 
		+ getOffset(temporaryEntry) 
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
		+ getOffset(secondOperand)
		+ ", " 
		+ getOffset(firstOperand)
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
		+ "\t" + getOffset(argument) 
		+ "\n";
}

void generateLastLabel()
{
	output += "lab" + convertIntToString(labelCounter - 1) + ":\n";
}

void generatePreviousLabel()
{
	output += "lab" + convertIntToString(labelCounter - 2) + ":\n";
}

void printOutput()
{
	cout << endl << endl << output << endl;
}
