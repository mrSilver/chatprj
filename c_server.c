/* c_server.c */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define BUFF_SIZE 65507
#define PORT_SIZE 6


void main(int argc, char *argv[])
{
    struct sockaddr_in myaddr;
    int servsock,clisock;
    char buffer[BUFF_SIZE];
    char server_port_no[PORT_SIZE + 1] = {0};
    unsigned int uint_serv_port_num;

    int c;

    while ((c = getopt (argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p':
                snprintf(server_port_no, PORT_SIZE, "%s", optarg);
                uint_serv_port_num = atoi(server_port_no);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option!\n");
                break;
        }
    }

    servsock = socket(AF_INET, SOCK_DGRAM, 0);
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(uint_serv_port_num);
    inet_pton(AF_INET, "192.168.8.99", &(myaddr.sin_addr));
    //myaddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(servsock, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        printf("Error on binding\n");
    }
    printf("\nServer ready,waiting for client....\n");
    printf("\nPort number %d\n", ntohs(myaddr.sin_port));
    char *some_addr;
    some_addr = inet_ntoa(myaddr.sin_addr);
    printf("%s\n", some_addr);
    while (1) {
         recvfrom(servsock, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);
         printf("\nClient:%s", buffer);
    }
}
