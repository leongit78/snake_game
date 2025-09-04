CC=cc
ASM=nasm
CFLAGS=-c -Wall
ASMFLAGS=-felf64 -o
SOURCES=main.c game.c show.c term.asm ch.asm
OBJECTS=main.o game.o show.o term.o ch.o
EXECUTABLE=game
	
.PHONY: all clean

all: $(SOURCES) ${OBJECTS} $(EXECUTABLE) 
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -no-pie $(OBJECTS) -o $@ -lncurses

${OBJECTS}: ${SOURCES}
	${CC} ${CFLAGS} main.c -o main.o
	${CC} ${CFLAGS} game.c -o game.o 
	${CC} ${CFLAGS} show.c -o show.o 
	${ASM} ${ASMFLAGS} ch.o ch.asm
	${ASM} ${ASMFLAGS} term.o term.asm

clean:
	rm -rf ${EXECUTABLE} ${OBJECTS}

