void initCodeGenerator();

void createLabel();
void createJump(string label);
int createExpression(int operationType, int firstOperandId, int secondOperandId);

int generateExpression(string operation, int & firstOperandId, int & secondOperandId);

void typeConversion(int & firstOperandId, int & secondOperandId);

void generateIntToReal(int & id);

string getSuffix(int type);

int generateAssignmentOperation(int firstOperandId, int secondOperandId);

void generateExit();

void generateProcedure(int procedureId, int argumentId);

void printOutput();
