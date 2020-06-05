CC=gcc
CFLAGS=-I.
 
all: MPClient server client  

client : 
	$(CC) -o client  -lpthread -std=c99 client.c $(CFLAGS)

MPClient : 
	$(CC) -o MPClient  -lpthread -std=c99 MPClient.c $(CFLAGS)

server : 
	$(CC) -o server  -lpthread -std=c99 server.c $(CFLAGS)

clean :
	rm server MPClient client 