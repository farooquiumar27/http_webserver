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
char request[5000];
char response[5000];
char header[11];
int len,i;
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
printf("MY SERVER IS READY TO ACCEPT DATA\n");
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to connect to client side\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=recv(clientSocketDescriptor,header,sizeof(header),0);
if(successCode<0)
{
printf("Unable to accept request\n");
closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}
len=0;
for(i=0;header[i]!=' ' && header[i]!='\0';i++)len=len*10+(header[i]-48);

for(request[0]='\0';strlen(request)!=len;)successCode=recv(clientSocketDescriptor,request,sizeof(request),0);
if(successCode>0)
{
printf("Request recived\n");
printf("%s\n",request);
}
strcpy(response,"WELCOME TO OF MY WORLD YOU HAVE ALREADY REGISTERED FOR MY COURSE");
len=strlen(response);
sprintf(header,"%-10d",len);
successCode=send(clientSocketDescriptor,header,sizeof(header)+1,0);
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