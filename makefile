CPP = g++
FLEX = flex
BISON = bison
HEADERS = global.h symbolTableManager.h lexer.h parser.h
SOURCES = main.cpp symbolTableManager.cpp lexer.c parser.c
OBJECTS = main.o symbolTableManager.o lexer.o parser.o
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
