void initCodeGenerator();
void printOutput();

int createExpression(int operationType, int firstOperandId, int secondOperandId);

int generateExpression(string operation, int firstOperandId, int secondOperandId);
int generateLogicalOperation(string operation, int firstOperandId, int secondOperandId);
int generateAssignmentOperation(int firstOperandId, int secondOperandId);

int createLabel();
void generateLabel(int labelId);
void generatePreviousLabel();
void generateSecondPreviousLabel();

void generateJump(string label);
void generateConditionalJump(string operation, int firstOperandId, int secondOperandId, int labelId);
void generateConditionalValueJump(string operation, int operandId, int number);

void generateExit();

void generateSubProgramEnter(int subProgramId);
void generateSubProgramLeave();
void generateSubProgramCall(int procedureId);
void generateProcedureCallByExpressionList(int procedureId);
void generateProcedureReadWriteCall(int procedureId, int argumentId);

int generateArrayElement(int arrayId, int argumentId);

void generateIntToReal(int & id);
void generateRealToInt(int & id);

int createTemporaryVariableEntry(int type);
int createNumberEntry(int number, int type);

void updateFunctionType(int functionId, int type);
void updateProcedureType(int procedureId);
void updateProcedureArguments(int type);

void addWhileLabel();
string getWhileLabel();

string getSuffix(int type);