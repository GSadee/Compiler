#include "global.h"

string output;
vector<int> whileLabels;

void initCodeGenerator()
{
	generateJump("lab0");
}

void printOutput()
{
	cout << endl << endl << output << endl;
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
			return generateLogicalOperation("je", firstOperandId, secondOperandId);
		case NOT_EQUAL:
			return generateLogicalOperation("jne", firstOperandId, secondOperandId);
		case GREATER:
			return generateLogicalOperation("jg", firstOperandId, secondOperandId);
		case LOWER:
			return generateLogicalOperation("jl", firstOperandId, secondOperandId);
		case GREATER_EQUAL:
			return generateLogicalOperation("jge", firstOperandId, secondOperandId);
		case LOWER_EQUAL:
			return generateLogicalOperation("jle", firstOperandId, secondOperandId);
		default:
			break;
	}

	return -1;
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

int generateLogicalOperation(string operation, int firstOperandId, int secondOperandId)
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
	generateJump(nextLabel.name);

	generateLabel(labelId);

	int number1Id = createNumberEntry(1, INTEGER_VALUE);

	generateAssignmentOperation(temporaryId, number1Id);

	generateLabel(nextLabelId);

	return temporaryId;
}

int createLabel()
{
	string label = "lab" + convertIntegerToString(labelCounter++);
    
    return addSymbolWithType(label, LABEL);
}

void generateLabel(int labelId)
{
	SymbolTableEntry label = getSymbol(labelId);
	output += label.name + ":\n";
}

void generatePreviousLabel()
{
	output += "lab" + convertIntegerToString(labelCounter - 1) + ":\n";
}

void generateSecondPreviousLabel()
{
	output += "lab" + convertIntegerToString(labelCounter - 2) + ":\n";
}

void generateJump(string label)
{
	output += "\tjump.i\t\t#" + label + "\n";
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

void generateConditionalValueJump(string operation, int operandId, int number)
{
	int labelId = createLabel();
	int numberId = createNumberEntry(number, INTEGER_VALUE);

	generateConditionalJump(operation, operandId, numberId, labelId);
}

void generateExit()
{
	output += "\texit\n";
}

void generateSubProgramEnter(int subProgramId) 
{
	SymbolTableEntry subProgram = getSymbol(subProgramId);

	output += subProgram.name + ":\n\tenter.i\t\t#?\n";
}

void generateSubProgramLeave()
{
	output += "\tleave\n\treturn\n";

	output.replace(output.find("?"), 1, convertIntegerToString(localOffset));
}

void generateSubProgramCall(int procedureId)
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
    			+ "\tpush.i\t\t" + getFromOffset(temporaryEntry) + "\n";
		} else {
			int entryId = temporaryArguments.at(i);
			typeConversionToFirstOperand(procedure.arguments.at(i), entryId);
			entry = getSymbol(entryId);
    		output += "\tpush.i\t\t" + getFromOffset(entry) + "\n";			
		}
	}
	temporaryArguments.clear();

    if (FUNCTION == procedure.type) {
    	int temporaryId = createTemporaryVariableEntry(procedure.returnType);
		SymbolTableEntry temporaryEntry = getSymbol(temporaryId);
		symbolTable.at(procedureId).returnOffset = temporaryEntry.offset;
    	output += "\tpush.i\t\t" + getFromOffset(temporaryEntry) + "\n";
    	size++;
    }

	output += "\tcall.i\t\t#" + procedure.name + "\n";
	if (0 < size) {
		output += "\tincsp.i\t\t#" + convertIntegerToString(size * 4) + "\n";	
	}
}


void generateProcedureCallByExpressionList(int procedureId)
{
    SymbolTableEntry procedure = getSymbol(procedureId);
    if ("write" == procedure.name || "read" == procedure.name) {
        while (0 < temporaryArguments.size()) {
            generateProcedureReadWriteCall(procedureId, temporaryArguments.front());
            temporaryArguments.erase(temporaryArguments.begin());
        }
    } else {
        generateSubProgramCall(procedureId);
    }
}

void generateProcedureReadWriteCall(int procedureId, int argumentId)
{
	SymbolTableEntry procedure = getSymbol(procedureId);
	SymbolTableEntry argument = getSymbol(argumentId);

	output += "\t" + procedure.name + getSuffix(argument.type)
		+ "\t\t" + getOffset(argument) 
		+ "\n";
}

int generateArrayElement(int arrayId, int argumentId)
{
	cout << endl << "arrayId: " << arrayId << endl;
	cout << endl << "argumentId: " << argumentId << endl;

	SymbolTableEntry array = getSymbol(arrayId);
	SymbolTableEntry argument = getSymbol(argumentId);

	if (REAL_VALUE == argument.type || REAL == argument.type) {
		generateRealToInt(argumentId);
	}

	argument = getSymbol(argumentId);

	int temporaryId = createTemporaryVariableEntry(INTEGER);
	SymbolTableEntry temporaryEntry = getSymbol(temporaryId);

	int temporaryId2 = createTemporaryVariableEntry(INTEGER);
	SymbolTableEntry temporaryEntry2 = getSymbol(temporaryId2);

	output += "\tsub.i\t\t" + getOffset(argument) 
		+ ", #1, " + getOffset(temporaryEntry) 
		+ "\n"
		+ "\tmul.i\t\t" + getOffset(temporaryEntry) 
		+ ", #" + convertIntegerToString(getVariableOffset(array.returnType)) 
		+ ", " + getOffset(temporaryEntry) 
		+ "\n"
		+ "\tadd.i\t\t" + getFromOffset(array)
		+ ", " + getOffset(temporaryEntry)
		+ ", " + getOffset(temporaryEntry2)
		+ "\n";

	symbolTable.at(temporaryId2).pointer = true;

	return temporaryId2;
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

int createTemporaryVariableEntry(int type)
{
	if (INTEGER_VALUE == type) {
		type = INTEGER;
	} else if (REAL_VALUE == type) {
		type = REAL;
	}

	string name = "$t" + convertIntegerToString(temporaryVariableCounter++);
	
	return addSymbolWithType(name, type);
}

int createNumberEntry(int number, int type)
{
	string name = convertIntegerToString(number);
	
	return addSymbolWithType(name, type);	
}

void updateFunctionType(int functionId, int type)
{
    symbolTable.at(functionId).type = FUNCTION;
    symbolTable.at(functionId).returnType = type;
}

void updateProcedureType(int procedureId)
{
    symbolTable.at(procedureId).type = PROCEDURE;
}

void updateProcedureArguments(int type)
{
    for (int i = 0; i < untypedTokens.size(); i++) {
        int id = untypedTokens.at(i);
        symbolTable.at(currentSubProgram).arguments.push_back(id);

        symbolTable.at(id).type = type;
        symbolTable.at(id).reference = true;
        symbolTable.at(id).offset = calculateReferenceOffset();

    }
    untypedTokens.clear();
}

void addWhileLabel()
{
	whileLabels.push_back(getSymbolId("lab" + convertIntegerToString(labelCounter - 1)));
}

string getWhileLabel()
{
	int labelId = whileLabels.back();
	SymbolTableEntry labelEntry = getSymbol(labelId);
	string label = labelEntry.name;
	whileLabels.pop_back();

	return label;
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
