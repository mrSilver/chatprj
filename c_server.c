/* c_server.c */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <signal.h>
#define BUFF_SIZE 65507
#define PORT_SIZE 6
#define IFACE_SIZE 15 //size of network interface name

volatile sig_atomic_t stop = 0;

void sig_handler(int signo)
{
   stop = 1;
   /*if (signo == SIGINT) {
   printf("\nSIGNAL CAUGHT\nstop = %d",stop);
   }*/
}

int main(int argc, char *argv[])
{

    struct sockaddr_in myaddr;
    int servsock,clisock;
    char buffer[BUFF_SIZE];
    char server_port_no[PORT_SIZE + 1] = {0};
    unsigned int uint_serv_port_num;
    char iface[IFACE_SIZE + 1];
    int c; //counter for options (argv[])

    while ((c = getopt (argc, argv, "p:i:")) != -1) {
        switch (c) {
            case 'p':
                snprintf(server_port_no, PORT_SIZE, "%s", optarg);
                uint_serv_port_num = atoi(server_port_no);
                break;
            case 'i':
                snprintf(iface, IFACE_SIZE, "%s", optarg);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option!\n");
                break;
        }
    }

    servsock = socket(AF_INET, SOCK_DGRAM, 0);

    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ioctl(servsock, SIOCGIFADDR, &ifr);
    char *ipaddr;
    ipaddr = inet_ntoa(( (struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);
    printf("%s - %s\n", iface, ipaddr);

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(uint_serv_port_num);
    inet_pton(AF_INET, ipaddr, &(myaddr.sin_addr));
    //myaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(servsock, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        printf("Error on binding\n");
    }
    printf("\nServer ready,waiting for client....\n");
    printf("\nPort number %d\n", ntohs(myaddr.sin_port));
    char *some_addr;
    some_addr = inet_ntoa(myaddr.sin_addr);
    printf("%s\n", some_addr);

    signal(SIGINT, sig_handler);

    while (!stop) {
         recvfrom(servsock, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);
         printf("\nClient:%s", buffer);
    }
    printf("loop has been left!");
    return 0;
}
