CC=gcc
CFLAGS=-I.
 
all: udpserver udpclient tcpserver tcpclient  

udpclient : 
	$(CC) -o udpclient -std=c99 udpclient.c $(CFLAGS)

tcplient : 
	$(CC) -o tcplient  -std=c99 tcplient.c $(CFLAGS)

udpserver : 
	$(CC) -o udpserver -std=c99 udpserver.c $(CFLAGS)

tcpserver : 
	$(CC) -o tcpserver -std=c99 tcpserver.c $(CFLAGS)

clean :
	rm tcpserver udpserver tcpclient udpclient