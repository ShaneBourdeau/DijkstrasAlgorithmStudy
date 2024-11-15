#make file for pal.c

CFILES = main.c dijkstra.c pq.c
OFILES=$(CFILES:.c=.o)
CC = mpicc 

pal: $(OFILES)
	$(CC) -o dijkstra $(OFILES) $(CFLAGS)

clean:
	/bin/rm -f dijkstra $(OFILES)
