#include "global.h"
#include "codeGenerator.h"

string output;

vector<int> whileLabels;

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
	output += "\tjump.i\t\t#" + label + "\n";
}

int createExpression(int operationType, int firstOperandId, int secondOperandId)
{
	switch (operationType) {
		case PLUS:
			return generateExpression("add", firstOperandId, secondOperandId);
		case MINUS:
			return generateExpression("sub", firstOperandId, secondOperandId);
		case MULTIPLICATION:
			return generateExpression("mul", firstOperandId, secondOperandId);
		case DIVISION:
			return generateExpression("div", firstOperandId, secondOperandId);
		case MODULO:
			return generateExpression("mod", firstOperandId, secondOperandId);
		case AND:
			return generateExpression("and", firstOperandId, secondOperandId);
		case OR:
			return generateExpression("or", firstOperandId, secondOperandId);
		case ASSIGNMENT_OPERATOR:
			return generateAssignmentOperation(firstOperandId, secondOperandId);
		case EQUAL:
			generateLogicalExpression("je", firstOperandId, secondOperandId);
			return 10;
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
	int number0Id = createNumberEntry(0, INTEGER_VALUE);

	generateAssignmentOperation(temporaryId, number0Id);

	int nextLabelId = createLabel();
	SymbolTableEntry nextLabel = getSymbol(nextLabelId);
	createJump(nextLabel.name);

	generateLabel(labelId);

	int number1Id = createNumberEntry(1, INTEGER_VALUE);

	generateAssignmentOperation(temporaryId, number1Id);

	generateLabel(nextLabelId);

	return temporaryId;
}

int createTemporaryVariableEntry(int type)
{
	if (INTEGER_VALUE == type) {
		type = INTEGER;
	} else if (REAL_VALUE == type) {
		type = REAL;
	}

	string name = "$t" + convertIntToString(temporaryVariableCounter++);
	
	return addSymbolWithType(name, type);
}

int createNumberEntry(int number, int type)
{
	string name = convertIntToString(number);
	
	return addSymbolWithType(name, type);	
}

void generateConditionalValueJump(string operation, int operandId, int number)
{
	int labelId = createLabel();
	int numberId = createNumberEntry(number, INTEGER_VALUE);

	generateConditionalJump(operation, operandId, numberId, labelId);
}

void generateConditionalJump(string operation, int firstOperandId, int secondOperandId, int labelId)
{
	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);
	SymbolTableEntry label = getSymbol(labelId);

	output += "\t"
		+ operation + getSuffix(firstOperand.type) + "\t\t" 
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

	output += "\t" + operation + getSuffix(temporaryEntry.type) + "\t\t" 
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

void generateRealToInt(int & id)
{
	SymbolTableEntry entry = getSymbol(id);

	int temporaryId = createTemporaryVariableEntry(INTEGER);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
	output += "\trealtoint.r\t" 
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
	typeConversionToFirstOperand(firstOperandId, secondOperandId);

	SymbolTableEntry firstOperand = getSymbol(firstOperandId);
	SymbolTableEntry secondOperand = getSymbol(secondOperandId);

	output += "\tmov" + getSuffix(firstOperand.type) + "\t\t" 
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

void generateProcedureReadWriteCall(int procedureId, int argumentId)
{
	SymbolTableEntry procedure = getSymbol(procedureId);
	SymbolTableEntry argument = getSymbol(argumentId);

	output += "\t" + procedure.name + getSuffix(argument.type)
		+ "\t\t" + getOffset(argument) 
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

void addWhileLabel()
{
	whileLabels.push_back(getSymbolId("lab" + convertIntToString(labelCounter - 1)));
}

string getWhileLabel()
{
	int labelId = whileLabels.back();
	SymbolTableEntry labelEntry = getSymbol(labelId);
	string label = labelEntry.name;
	whileLabels.pop_back();

	return label;
}

void printOutput()
{
	cout << endl << endl << output << endl;
}

void generateSubProgramEnter(int subProgramId) 
{
	SymbolTableEntry subProgram = getSymbol(subProgramId);

	output += subProgram.name + ":\n\tenter.i\t\t#?\n";
}

void generateSubProgramLeave()
{
	output += "\tleave\n\treturn\n";

	output.replace(output.find("?"), 1, convertIntToString(localOffset));
}

void generateProcedureCall(int procedureId)
{
	SymbolTableEntry procedure = getSymbol(procedureId);

	if (FUNCTION != procedure.type && PROCEDURE != procedure.type) {
		return;
	}

	int size = temporaryArguments.size();

	for (int i = 0; i < temporaryArguments.size(); i++) {
		SymbolTableEntry entry = getSymbol(temporaryArguments.at(i));
		if (INTEGER_VALUE == entry.type || REAL_VALUE == entry.type) {
			int temporaryId = createTemporaryVariableEntry(entry.type);
			SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
    		output += "\tmov" + getSuffix(temporaryEntry.type) 
    			+ "\t\t#" + entry.name + ", " + getOffset(temporaryEntry) + "\n"
    			+ "\tpush.i\t\t#" + convertIntToString(temporaryEntry.offset) + "\n";
		} else {
			int entryId = temporaryArguments.at(i);
			typeConversionToFirstOperand(procedure.arguments.at(i), entryId);
			entry = getSymbol(entryId);
    		output += "\tpush.i\t\t#" + convertIntToString(entry.offset) + "\n";			
		}
	}
	temporaryArguments.clear();

    if (FUNCTION == procedure.type) {
    	int temporaryId = createTemporaryVariableEntry(procedure.returnType);
		SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
		symbolTable.at(procedureId).returnOffset = temporaryEntry.offset;
    	output += "\tpush.i\t\t#" + convertIntToString(temporaryEntry.offset) + "\n";
    	size++;
    }

	output += "\tcall.i\t\t#" + procedure.name + "\n";
	if (0 < size) {
		output += "\tincsp.i\t\t#" + convertIntToString(size * 4) + "\n";	
	}
}
