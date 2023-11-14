CFLAGS=-g -W -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBRARIES=-lADTs

exerciseQueue: exerciseQueue.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBRARIES)

exerciseQueueADT: exerciseQueue.o llistqueue.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBRARIES)

exerciseQueue.o: exerciseQueue.c

llistqueue.o: llistqueue.c

clean:
	rm -f *.0 exerciseQueue exerciseQueueADT
