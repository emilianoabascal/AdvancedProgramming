
OBJECTS = simpleTest.o stringTools.o
MAIN = simpleTest
DEPENDS = stringTools.h
CFLAGS = -std=gnu99 -Wall -pedantic -g
LDFLAGS = -lm
CC = gcc
simpleTest: simpleTest.o stringTools.o
		$(CC) $^ -o $@ $(LDFLAGS)
%.o: %.c $(DEPENDS)
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -rf $(MAIN) *.o

zip:
	zip $(MAIN).zip *
