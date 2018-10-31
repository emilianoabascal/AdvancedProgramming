# Simple example of a Makefile
# Emiliano Abascal Gurria A01023234

### Variables for this project ###
OBJECTS = fatal_error.o
# The header files
DEPENDS = fatal_error.h
# The executable programs to be created
CLIENT = pi_client
SERVER = pi_server
### Variables for the compilation rules ###
# The compiler program to use
CC = gcc
# Options to use when compiling object files
CFLAGS = -Wall -g -std=gnu99 -pedantic # -O2
# Options to use for the final linking process
# This one links the math library
LDLIBS = -lm
### The rules ###
# Default rule
all: $(CLIENT) $(SERVER)
# Rule to make the client program
$(CLIENT): $(CLIENT).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the server program
$(SERVER): $(SERVER).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
# Rule to make the object files
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)
# Clear the compiled files
clean:
	rm -rf *.o $(CLIENT) $(SERVER)
# Indicate the rules that do not refer to a file
.PHONY: clean all
