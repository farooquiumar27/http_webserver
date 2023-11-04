#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

int main( )
{
char request[5000],response[5000];
int serverSocketDescriptor;
int clientSocketDescriptor;
int successCode,len;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create the socket\n");
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(5050);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind the socket\n");
close(serverSocketDescriptor);
return 0;
}
listen(serverSocketDescriptor,10);
printf("MY SERVER IS READY TO ACCEPT THE REQUEST\n");
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to connet to client\n");
close(serverSocketDescriptor);
return 0;
}
successCode=recv(clientSocketDescriptor,request,sizeof(request),0);
if(successCode>0)
{
printf("Request recived\n");
}
sprintf(response,"WELCOME TO MY WORLD");
successCode=send(clientSocketDescriptor,response,strlen(response)+1,0);
if(successCode<0)
{
printf("Unable to send the response\n");
}
close(clientSocketDescriptor);
close(serverSocketDescriptor);
return 0;
}