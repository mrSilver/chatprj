# Makefile for Simple UDP Chat Project

c_client: c_client.o c_server 
	gcc -o c_client c_client.o

c_server: c_server.o
	gcc -o c_server c_server.o

c_client.o: c_client.c
	gcc -c c_client.c

c_server.o: c_server.c
	gcc -c c_server.c

clean:
	rm *.o c_client c_server

