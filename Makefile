# Makefile for Simple UDP Chat Project

c_client: c_client.o c_server 
	gcc -o c_client c_client.o

c_server: c_server.o sig_handler.o
	gcc -o c_server c_server.o sig_handler.o

c_client.o: c_client.c
	gcc -c c_client.c

c_server.o: c_server.c
	gcc -c c_server.c

sig_handler.o: sig_handler.c
	gcc -c sig_handler.c

clean:
	rm *.o c_client c_server

