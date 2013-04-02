/* c_client.c */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#define BUFF_SIZE 65507
#define PORT_SIZE 6
#define IP_SIZE 16

volatile sig_atomic_t stop = 0;

void sig_handler(int signo)
{
    stop = 1;
}

void main( int argc, char *argv[])
{
    struct sockaddr_in server;
    int udp_socket;
    int length;
    char server_port_number[PORT_SIZE + 1] = {0};
    char buffer[BUFF_SIZE];
    unsigned int uint_spn;
    char server_ip[IP_SIZE + 1];

    int c;

    while ((c = getopt (argc, argv, "p:i:")) != -1) {
        switch (c) {
            case 'i':
            snprintf(server_ip, IP_SIZE, "%s", optarg);
            break;
        case 'p':
            snprintf(server_port_number, PORT_SIZE, "%s", optarg);
            uint_spn = atoi(server_port_number);
            break;
        case '?':
            fprintf(stderr, "Unrecognized option!\n");
            break;
       }
    }
    printf("\nSPN = %u", uint_spn);

    /*printf("\nInput server ip address:");
    fgets(server_ip, IP_SIZE - 1, stdin);

    printf("\nInput server port number:");
    fgets(server_port_number, PORT_SIZE - 1, stdin);
    uint_spn = atoi(server_port_number);*/
    //printf("\nSPN = %u", uint_spn);

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(uint_spn);
    //printf("\nServer Port Number %d\n", ntohs(server.sin_port));
    // shows server port number
    inet_aton(server_ip, &server.sin_addr);

    printf("\nClient ready....\n");
    length = sizeof(server);
    signal(SIGINT, sig_handler);
    while (!stop) {
        printf("\nClient:");
        fgets(buffer, BUFF_SIZE - 1, stdin);
        sendto(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, length);
    }
    printf("Signal received, exiting...\n");
}
