#include "global.h"

void printSymbols()
{
	cout << "; " << "Symbol table dump" << endl;
	for (int iterator = 0; iterator < symbolTable.size(); iterator++) {
		SymbolTableEntry entry = symbolTable[iterator];
		
		cout << "; " << iterator << " ";
		if (SCOPE_GLOBAL == entry.scope) {
			cout << "Global ";
		} else {
			cout << "Local ";
		}

		switch (entry.type) {
			case INTEGER:
				cout << "variable " << entry.name << " integer ";
				break;
			case REAL:
				cout << "variable " << entry.name << " real ";
				break;
			case PROCEDURE:
				cout << "procedure " << entry.name << " ";
				break;
			case LABEL:
				cout << "label " << entry.name << " ";
				break;
			default:
				cout << entry.name << " ";
				break;
		}

		if (-1 < entry.offset) {
			cout << "offset=" << entry.offset;
		}

		cout << endl;
	}
}
