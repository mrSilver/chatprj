#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main()
{
 struct sockaddr_in myaddr;
 int servsock,clisock;
 char b1[100];
 servsock=socket(AF_INET,SOCK_DGRAM,0);
 myaddr.sin_family=AF_INET;
 myaddr.sin_port=2000;
 myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
 bind(servsock,(struct sockaddr *)&myaddr,sizeof(myaddr));
 printf("\nServer ready,waiting for client....\n");
while (1)
{
 recvfrom(servsock,b1,sizeof(b1),0,(struct sockaddr *) NULL,NULL);
 if(!(strcmp(b1,"end")))
 break;
 printf("\nClient:%s",b1);   
}
}
