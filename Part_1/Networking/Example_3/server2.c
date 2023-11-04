#include<stdio.h>
#include<windows.h>
#include<string.h>

int main( )
{
WORD ver;
WSADATA wsaData;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
char request[5000];
char response[5000];
char header[5000];
char responseLength[11];
int len,successCode;

ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return 0;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(5050);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind the server to port\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
}
listen(serverSocketDescriptor,10);
printf("MY SERVER IS READY TO ACCEPT REQUEST\n");
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unalbe to connet to client\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=recv(clientSocketDescriptor,request,sizeof(request),0);
if(successCode>0)
{
printf("Request accepted\n");
printf("%s\n",request);
}
strcpy(response,"<!DOCTYPE HTML>");
strcat(response,"<html lang='en'>");
strcat(response,"<meta charset='utf-8'>");
strcat(response,"<head>");
strcat(response,"<title>WHATEVER</title>");
strcat(response,"</head>");
strcat(response,"<body>");
strcat(response,"<h1>Welcome to my world .Its time to playy the GAMEEE</h1>");
strcat(response,"</body>");
strcat(response,"</html>");
strcpy(header,"HTTP/1.1 200 OK\n");
strcat(header,"Content-Type : text/html\n");
strcat(header,"Content-Size : ");
sprintf(responseLength,"%d\n",strlen(response)+1);
strcat(header,responseLength);
strcat(header,"\n");
successCode=send(clientSocketDescriptor,header,strlen(header)+1,0);
successCode=send(clientSocketDescriptor,response,strlen(response)+1,0);
closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}