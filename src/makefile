#makefile to compile the .C files in this directory
#and write the .o output to the ../obj directory
#This makefile also creates the yess executable.
CC = g++
INC = ../inc
BIN = ../bin
OBJD = ../obj
CFLAGS = -g -c -Wall -Wno-unused-variable -std=c++11 -I $(INC)
OBJ = $(OBJD)/ConditionCodes.o $(OBJD)/CPU.o $(OBJD)/D.o $(OBJD)/ExecuteStage.o \
	  $(OBJD)/F.o $(OBJD)/FetchStage.o $(OBJD)/Loader.o $(OBJD)/M.o \
	  $(OBJD)/Memory.o $(OBJD)/MemoryStage.o $(OBJD)/PipeReg.o $(OBJD)/PipeRegField.o \
	  $(OBJD)/RegisterFile.o $(OBJD)/Stage.o $(OBJD)/String.o $(OBJD)/Tools.o \
	  $(OBJD)/W.o $(OBJD)/E.o $(OBJD)/WritebackStage.o $(OBJD)/yess.o $(OBJD)/DecodeStage.o

# OBJ = $(OBJD)/Tools.o $(OBJD)/Memory.o $(OBJD)/ConditionCodes.o \
#       $(OBJD)/RegisterFile.o $(OBJD)/String.o $(OBJD)/yess.o\
# 	  $(OBJD)/Loader.o $(OBJD)/CPU.o $(OBJD)/D.o \
# 	  $(OBJD)/E.o $(OBJD)/F.o $(OBJD)/M.o $(OBJD)/W.o $(OBJD)/PipeReg.o \
# 	  $(OBJD)/PipeRegField.o $(OBJD)/DecodeStage.o \
# 	  $(OBJD)/ExecuteStage.o $(OBJD)/MemoryStage.o $(OBJD)/WritebackStage.o \
# 	  $(OBJD)/FetchStage.o $(OBJD)/Stage.o


#rule to match .C with object in another directory
$(OBJD)/%.o: %.C
	@mkdir -p $(OBJD)
	$(CC) $(CFLAGS) $< -o $@

#1) Need a .o target for the new .C files (String.C, yess.C, Loader.C)
#   Add those targets to the OBJ macro.
#
#2) Need a ../bin/yess target that links together the .o files
#   to create a ../bin/yess executable (make the first target in
#   the makefile).  It actually doesn't need all of the .o files
#   yet, but it doesn't hurt to link them into the executable
#   Use the OBJ and BIN macros

#The rules below should match what you had in your old makefile

run:
	make $(BIN)/yess
	$(BIN)/run.sh

$(BIN)/yess: $(OBJ)
	$(CC) $(OBJ) -o $(BIN)/yess

$(OBJD)/Memory.o: $(INC)/Tools.h $(INC)/Memory.h

$(OBJD)/Tools.o: $(INC)/Tools.h

$(OBJD)/RegisterFile.o: $(INC)/RegisterFile.h $(INC)/Tools.h

$(OBJD)/ConditionCodes.o: $(INC)/ConditionCodes.h $(INC)/Tools.h

$(OBJD)/String.o: $(INC)/Debug.h $(INC)/String.h

$(OBJD)/yess.o:  $(INC)/Debug.h $(INC)/Memory.h $(INC)/String.h $(INC)/Loader.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/ConditionCodes.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Stage.h $(INC)/CPU.h

$(OBJD)/Loader.o: $(INC)/Debug.h $(INC)/Memory.h $(INC)/String.h $(INC)/Loader.h $(INC)/Tools.h

$(OBJD)/CPU.o: $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h $(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/MemoryStage.h $(INC)/ExecuteStage.h $(INC)/DecodeStage.h $(INC)/FetchStage.h $(INC)/WritebackStage.h $(INC)/CPU.h

$(OBJD)/D.o: $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/D.h $(INC)/Status.h

$(OBJD)/E.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/E.h $(INC)/Status.h

$(OBJD)/F.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h

$(OBJD)/M.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/M.h $(INC)/Status.h

$(OBJD)/W.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/W.h $(INC)/Status.h

$(OBJD)/PipeReg.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h

$(OBJD)/PipeRegField.o: $(INC)/PipeRegField.h

$(OBJD)/DecodeStage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/Stage.h $(INC)/DecodeStage.h $(INC)/Debug.h $(INC)/E.h $(INC)/D.h $(INC)/M.h $(INC)/W.h $(INC)/Instructions.h

$(OBJD)/ExecuteStage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/Stage.h $(INC)/ExecuteStage.h $(INC)/Instructions.h $(INC)/Tools.h $(INC)/Debug.h $(INC)/Status.h $(INC)/E.h $(INC)/M.h $(INC)/W.h

$(OBJD)/MemoryStage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/Stage.h $(INC)/MemoryStage.h $(INC)/Debug.h $(INC)/M.h $(INC)/W.h $(INC)/Instructions.h $(INC)/Status.h 

$(OBJD)/WritebackStage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/Stage.h $(INC)/WritebackStage.h $(INC)/Debug.h $(INC)/Instructions.h $(INC)/W.h $(INC)/Status.h

$(OBJD)/FetchStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h $(INC)/Stage.h $(INC)/FetchStage.h $(INC)/Status.h $(INC)/Debug.h $(INC)/E.h $(INC)/M.h $(INC)/W.h

$(OBJD)/Stage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/Stage.h

clean:
	rm -f $(OBJ)
