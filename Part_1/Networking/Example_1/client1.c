#include<stdio.h>
#include<string.h>
#include<windows.h>
int main( )
{
WSADATA wsaData;
WORD ver;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
int successCode;
char request[5000],response[5000];
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
clientSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(clientSocketDescriptor<0)
{
printf("Unalbe to create socket\n");
WSACleanup( );
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(5050);
serverSocketInformation.sin_addr.s_addr=inet_addr("127.0.0.1");
successCode=connect(clientSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to connect to server\n");
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}
strcpy(request,"ITS TIME TOO PLAYY THE GAMMEEEE !!!! AND THE RACE IS NOT OVER YET");
successCode=send(clientSocketDescriptor,request,strlen(request)+1,0);
if(successCode<0)
{
printf("Unable to send the request\n");
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=recv(clientSocketDescriptor,response,sizeof(response),0);
if(successCode>0)
{
printf("Response recived\n");
printf("%s\n",response);
}
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}