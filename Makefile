CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS = -lglut -lGLU -lGL -lm -lGLEW
EXEC = main
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)
	
.c.o: $(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC) $(OBJECTS)

