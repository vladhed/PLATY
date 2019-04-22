CC = gcc
CC_FLAGS = -g -ansi -pedantic -Wall
FILES = buffer.c platy.c scanner.c parser.c
OUT_EXE = parser

build: $(FILES)
	$(CC) $(CC_FLAGS) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core *.exe

rebuild: clean build

buffer.c: buffer.h
scanner.c: table.h token.h buffer.h
parser.c: parser.h
platy_st.c: buffer.h token.h

