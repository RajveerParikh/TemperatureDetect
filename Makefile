all: cit595

cit595: arduino.c
	clang arduino.c -o arduino

clean: 
	rm -rf *.o

clobber: clean
	rm -rf dchat