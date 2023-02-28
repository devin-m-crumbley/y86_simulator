#lab6 makefile (complete)
BIN = ../bin

run:
	(cd src && make $(BIN)/yess)
	(cd bin && ./run.sh)

clean:
	(cd bin && rm -f lab4 & rm -f lab5 & rm -f yess)
	(cd obj && rm -f *.o)
