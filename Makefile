all : client server

client : client.o linkedlist.o
	gcc -o client -g client.o linkedlist.o

client.o : client.c linkedlist.h
	gcc -g -c -Wall client.c

server : server.o linkedlist.o
	gcc -o server -g server.o linkedlist.o

server.o : server.c linkedlist.h
	gcc -g -c -Wall server.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -g -c -Wall linkedlist.c
clean :
	rm -f *.o server client
