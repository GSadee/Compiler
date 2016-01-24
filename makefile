CPP = g++
FLEX = flex
BISON = bison
HEADERS = global.h symbolTableManager.h symbolTablePrinter.h codeGenerator.h offsetCalculator.h converter.h lexer.h parser.h
SOURCES = main.cpp symbolTableManager.cpp symbolTablePrinter.cpp codeGenerator.cpp offsetCalculator.cpp converter.cpp lexer.c parser.c
OBJECTS = main.o symbolTableManager.o symbolTablePrinter.o codeGenerator.o offsetCalculator.o converter.o lexer.o parser.o
LEXER = lexer.c lexer.h
PARSER = parser.c parser.h
CFLAGS = -c -pedantic -g

RESULT = compiler

$(RESULT) : $(OBJECTS)
	$(CPP) $^ -o $@ -lfl

$(OBJECTS) : $(SOURCES) $(HEADERS)
	$(CPP) $(CFLAGS) $(SOURCES)

parser.c : parser.y
	$(BISON) -o $@ -d $^

lexer.c : lexer.l
	$(FLEX) -o $@ --header-file=lexer.h $^

.PHONY : clean

clean:
	-rm -f $(RESULT) $(OBJECTS) $(LEXER) $(PARSER)
