CC=gcc
CFLAGS=-D_FORTIFY_SOURCE=2 -Wall  -g3 -std=c17 -pedantic 

SRC=src
BIN=bin
SRCS=$(wildcard $(SRC)/*.c)
BINS=$(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRCS))

EXECUTABLE=ERTMS

all: $(EXECUTABLE)

$(EXECUTABLE):  $(BINS)
	$(CC) $(CFLAGS)  $^ -o $@

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: setup clean

setup: 
	mkdir -p tmp
	mkdir -p log
	mkdir -p segments
	mkdir -p $(BIN)
	#mkdir -p $(SRC)
	#unzip mappe.zip -d segments
	#unzip source.zip -d .

clean:
	$(RM) ./bin/*.o
