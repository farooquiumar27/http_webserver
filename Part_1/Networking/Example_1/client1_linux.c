#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

int main( )
{
char request[5000],response[5000];
int clientSocketDescriptor;
int successCode,len;
struct sockaddr_in serverSocketInformation;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create the socket\n");
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(5050);
serverSocketInformation.sin_addr.s_addr=inet_addr("127.0.0.1");
successCode=connet(clientSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to connect the socket\n");
close(clientSocketDescriptor);
return 0;
}
sprintf(request,"ITS TIME TO PLAYY THE GAMEE !!!! AND THE RACE IS NOT OVER YEAT");
successCode=send(clientSocketDescriptor,request,strlen(request)+1,0);
if(successCode>0)
{
printf("Request recived\n");
}
successCode=send(clientSocketDescriptor,response,sizeof(response),0);
if(successCode<0)
{
printf("Unable to send the response\n");
}
close(clientSocketDescriptor);
return 0;
}