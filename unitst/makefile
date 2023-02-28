#lab5 makefile (complete)
CC = g++
INC = ../inc
BIN = ../bin
OBJD = ../obj
CFLAGS = -g -c -Wall -std=c++11 -I $(INC)
OBJ = $(OBJD)/lab4.o $(OBJD)/MemoryTester.o $(OBJD)/Memory.o $(OBJD)/Tools.o \
$(OBJD)/RegisterFile.o $(OBJD)/RegisterFileTester.o $(OBJD)/ConditionCodes.o \
$(OBJD)/ConditionCodesTester.o

$(OBJD)/%.o: %.C
	@mkdir -p $(OBJD)
	$(CC) $(CFLAGS) $< -o $@

help:
	@echo "This makefile can be used to compile and execute unit tests."
	@echo "Type either 'make lab4' OR 'make lab5'"

lab5:
	(cd ../src && make $(OBJD)/String.o)
	make runlab5

lab4:
	(cd ../src && make $(OBJD)/Memory.o && make $(OBJD)/RegisterFile.o && \
make $(OBJD)/ConditionCodes.o && make $(OBJD)/Tools.o)
	make runlab4

runlab4:
	make $(BIN)/lab4
	$(BIN)/lab4

runlab5:
	make $(BIN)/lab5
	$(BIN)/lab5

$(BIN)/lab5: $(OBJD)/lab5.o $(OBJD)/String.o
	$(CC) $(OBJD)/lab5.o $(OBJD)/String.o -o $(BIN)/lab5

$(BIN)/lab4: $(OBJ)
	$(CC) $(OBJ) -o $(BIN)/lab4

$(OBJD)/lab5.o: $(INC)/String.h

$(OBJD)/lab4.o: $(INC)/MemoryTester.h $(INC)/Memory.h

$(OBJD)/MemoryTester.o: $(INC)/MemoryTester.h $(INC)/Memory.h

$(OBJD)/RegisterFileTester.o: $(INC)/RegisterFile.h $(INC)/RegisterFileTester.h

$(OBJD)/ConditionCodesTester.o: $(INC)/ConditionCodes.h $(INC)/ConditionCodesTester.h

clean:
	rm -f ../bin/lab4
	rm -f ../bin/lab5
	rm -f $(OBJD)/*

