void initCodeGenerator();

void createLabel();
void createJump(string label);
void createExpression(int operationType, int firstOperandId, int secondOperandId);

void generateExpression(string operation, int & firstOperandId, int & secondOperandId);

void typeConversion(int & firstOperandId, int & secondOperandId);

void generateIntToReal(int & id);

void printOutput();
