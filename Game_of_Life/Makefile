# Makefile to compile a complex program composed of many libraries
#Emiliano Abascal Gurria
#A01023234
#11/19/2018
### Variables for this project ###
# These should be the only ones that need to be modified
# The files that must be compiled, with a .o extension
OBJECTS = gameOfLife.o string_functions.o pgm_image.o
# The header files
DEPENDS = string_functions.h pgm_image.h gameFunctions.h
# The executable program to be created
MAIN = gameOfLife

### Variables for the compilation rules ###
# These should work for most projects, but can be modified when necessary
# The compiler program to use
CC = gcc-8
# Options to use when compiling object files
CFLAGS = -Wall -g -std=c99 -pedantic -fopenmp # -O2
# Options to use for the final linking process
# This one links the math library
LDLIBS = -lm -fopenmp

### The rules ###
# These should work for most projects without change

# Default rule
all: $(MAIN)

# Rule to make the executable
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the object files
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

# Clear the compiled files
clean:
	rm -rf *.o $(MAIN)

# Indicate the rules that do not refer to a file
.PHONY: clean all
