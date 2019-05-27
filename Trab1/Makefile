CFLAGS = -Wall -g
OBJECTS = arkread.o arkwrite.o main.o

all: $(OBJECTS) arkread.h arkwrite.h
	gcc $(CFLAGS) $(OBJECTS) -o programaTrab1
run: 
	./programaTrab1
zip:
	zip programaTrab1 *.c *.h Makefile