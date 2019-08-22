CC=gcc
CFLAGS = -Werror -Wall
MAIN= tasteful.o
OBJ= test_example.o
AUX= testing_suite.c

all: $(OBJ) $(MAIN)
	$(CC) $(CFLAGS) -o $(MAIN:.o=) $(MAIN) $(AUX)

$(OBJ): $(OBJ:.o=.c) $(AUX) 
	$(CC) $(CFLAGS) -c $(@:.o=.c) $(AUX)
	$(CC) $(CFLAGS) -o $(@:.o=.tasty) $@ $(AUX)

clean:
	rm -rf $(OBJ) $(AUX:.c=.o) $(OBJ:.o=) $(OBJ:.o=.tasty) $(MAIN) $(MAIN:.o=)
