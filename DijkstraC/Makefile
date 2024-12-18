# Make file for DijkstraC
#
# Tyler Knapp and Shane Bourdeau

CFILES = main.c dijkstra.c pq.c
HFILES = dijkstra.h
OFILES = $(CFILES:.c=.o)

# Compiler
CC = clang

# Flags
CFLAGS = -Wall 

# Main target
dijkstra: $(OFILES)
	$(CC) -o dijkstra $(OFILES) $(CFLAGS)

# Dependencies
main.o: main.c $(HFILES)
	$(CC) -c main.c $(CFLAGS)

dijkstra.o: dijkstra.c $(HFILES)
	$(CC) -c dijkstra.c $(CFLAGS)

pq.o: pq.c
	$(CC) -c pq.c $(CFLAGS)

# Clean up
clean:
	/bin/rm -f dijkstra $(OFILES)
