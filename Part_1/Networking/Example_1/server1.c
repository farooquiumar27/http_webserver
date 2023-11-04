#include<stdio.h>
#include<string.h>
#include<windows.h>
int main( )
{
WSADATA wsaData;
WORD ver;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int successCode;
int len;
char request[5000];
char response[5000];
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData); //library from scoket initialized
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
WSACleanup( );
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(5050);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind the server\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}
listen(serverSocketDescriptor,10);
printf("MY SERVER IS READY TO ACCEPT REQUEST\n");
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to connect to client side\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=recv(clientSocketDescriptor,request,sizeof(request),0);
if(successCode>0)
{
printf("Request recived\n");
printf("%s\n",request);
}
strcpy(response,"WELCOME TO OF MY WORLD");
successCode=send(clientSocketDescriptor,response,strlen(response)+1,0);
if(successCode<0)
{
printf("Unable to send response\n");
}
closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}