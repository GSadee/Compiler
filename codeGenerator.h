void initCodeGenerator();

int createLabel();
void createJump(string label);
int createExpression(int operationType, int firstOperandId, int secondOperandId);

int generateExpression(string operation, int firstOperandId, int secondOperandId);
int generateLogicalExpression(string operation, int firstOperandId, int secondOperandId);

void generateConditionalJump(string operation, int firstOperandId, int secondOperandId, int labelId);
void generateConditionalValueJump(string operation, int operandId, int number);

void typeConversion(int & firstOperandId, int & secondOperandId);

void generateIntToReal(int & id);

string getSuffix(int type);

int generateAssignmentOperation(int firstOperandId, int secondOperandId);

void generateExit();

void generateProcedure(int procedureId, int argumentId);
void generateLabel(int labelId);

void printOutput();

int createTemporaryVariableEntry(int type);
int createNumberEntry(int number);

string getOffset(SymbolTableEntry entry);
void generateLastLabel();
void generatePreviousLabel();
