#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main()
{
 struct sockaddr_in server;
 int s,n;
 char b2[100];
 s=socket(AF_INET,SOCK_DGRAM,0);
 server.sin_family=AF_INET;
 server.sin_port=2000;
 server.sin_addr.s_addr=inet_addr("127.0.0.1");
 printf("\nClient ready....\n");
 n=sizeof(server);
 while (1)
 {
 printf("\nClient:");
 gets(b2);
 sendto(s,b2,sizeof(b2),0,(struct sockaddr *)&server,n);
 if(strcmp(b2,"end")==0)
  break;
 }
}
