#include "global.h"

void printSymbols()
{
	cout << endl << "; " << "Symbol table dump" << endl;
	for (int i = 0; i < symbolTable.size(); i++) {
		SymbolTableEntry entry = symbolTable.at(i);
		
		cout << "; " << i << " ";
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
			case INTEGER_VALUE:
				cout << "number " << entry.name << " integer ";
				break;
			case REAL_VALUE:
				cout << "number " << entry.name << " real ";
				break;
			case PROCEDURE:
				cout << "procedure " << entry.name << " ";
				break;
			case FUNCTION:
				cout << "function " << entry.name;
				if (INTEGER == entry.returnType) {
					cout << " integer ";
				} else if (REAL == entry.returnType) {
					cout << " real ";
				}
				break;
			case LABEL:
				cout << "label " << entry.name << " ";
				break;
			case ARRAY:
				cout << "array " << entry.name << " [" << entry.startIndex << ".." << entry.endIndex << "] of ";
				if (INTEGER == entry.returnType) {
					cout << "integer ";
				} else if (REAL == entry.returnType) {
					cout << "real ";
				}
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
