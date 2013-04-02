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
#include <sys/time.h>
#include <stdlib.h>
#define BUFF_SIZE 65507
#define PORT_SIZE 6
#define IFACE_SIZE 15 //size of network interface name
#define SOCK_SIZE 3

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
    char srvsocks[SOCK_SIZE + 1];
    int c; //counter for options (argv[])

    while ((c = getopt (argc, argv, "p:i:n:")) != -1) {
        switch (c) {
            case 'p':
                snprintf(server_port_no, PORT_SIZE, "%s", optarg);
                uint_serv_port_num = atoi(server_port_no);
                break;
            case 'i':
                snprintf(iface, IFACE_SIZE, "%s", optarg);
                break;
            case 'n':
                snprintf(srvsocks, SOCK_SIZE, "%s", optarg);
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
    //printf("%s - %s\n", iface, ipaddr);





    int ports_count;
    ports_count = atoi(srvsocks);
    printf("Ports count = %d\n", ports_count);

    int sockfds[ports_count];
    struct sockaddr_in srvaddr[ports_count];

    fd_set rfds;
    FD_ZERO(&rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    int retval;

    int counter = 0;
    for (counter = 0; counter < ports_count; counter++) {
        sockfds[counter] = socket(AF_INET, SOCK_DGRAM, 0);
        srvaddr[counter].sin_family = AF_INET;
        srvaddr[counter].sin_port = htons(uint_serv_port_num + counter);
        inet_pton(AF_INET, ipaddr, &(srvaddr[counter].sin_addr));
        bind(sockfds[counter], (struct sockaddr *) &srvaddr[counter], sizeof(srvaddr[counter]));
        printf("\nPort number %d\n", ntohs(srvaddr[counter].sin_port));
        //fcntl(sockfds[counter], F_SETFL, O_NONBLOCK);
    }

    int ctr = 0;
    int max_fd = sockfds[ctr];

    for (ctr = 0; ctr < ports_count; ctr++) {
        if (max_fd < sockfds[ctr]) {
            max_fd = sockfds[ctr];
        }
    }
    max_fd = max_fd + 1;

    char *some_addr;
    some_addr = inet_ntoa(srvaddr[0].sin_addr);
    printf("%s\n", some_addr);
    printf("\nServer ready,waiting for client....\n");




/*
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(uint_serv_port_num);
    inet_pton(AF_INET, ipaddr, &(myaddr.sin_addr));

    if (bind(servsock, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        printf("Error on binding\n");
    }
*/

    signal(SIGINT, sig_handler);


    while (!stop) {
        int ctr1 = 0;
        FD_ZERO(&rfds);
        int ctr2 = 0;
        for (ctr2 = 0; ctr2 < ports_count; ctr2++) {
            FD_SET(sockfds[ctr2], &rfds);
        }
        retval = select( max_fd, &rfds, NULL, NULL, &tv);
        if (retval < 0) {
            printf("\nSelect thrown an exception\n");
            return 0;
        }
        for (ctr1 = 0; ctr1 < ports_count; ctr1++) {
            if (FD_ISSET(sockfds[ctr1], &rfds)) {
                recvfrom(sockfds[ctr1], buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);
                printf("\nClient[%d]:%s", ctr1, buffer);
            }
        }
    }





/*    int crazy_counter = 0;
    while (!stop) {
         for (crazy_counter = 0; crazy_counter < ports_count; crazy_counter++) {
         recvfrom(sockfds[crazy_counter], buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);
         printf("\nClient:%s", buffer);
         }
         crazy_counter = 0;
    }
*/
    printf("loop has been left!");
    return 0;
}
