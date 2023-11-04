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
char request[5000],response[5000],header[11];
int roll,len,i;
char gender;
char name[51];
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
printf("Enter the roll number: ");
scanf("%d",&roll);
fflush(stdin);
printf("Enter the name : ");
fgets(name,21,stdin);
fflush(stdin);
name[strlen(name)-1]='\0';
printf("Enter gender : ");
scanf("%c",&gender);
successCode=connect(clientSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to connect to server\n");
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}
sprintf(request,"%d,%s,%c",roll,name,gender);
len=strlen(request);
sprintf(header,"%-10d",len);
successCode=send(clientSocketDescriptor,header,strlen(header)+1,0);
if(successCode<0)
{
printf("Unable to send the request\n");
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=send(clientSocketDescriptor,request,strlen(request)+1,0);
if(successCode<0)
{
printf("Unable to send the request\n");
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}
successCode=recv(clientSocketDescriptor,header,sizeof(header),0);
len=0;
for(i=0;header[i]!=' ' && header[i]!='\0';i++)len=len*10+(header[i]-48);
for(response[0]='\0';strlen(response)!=len;)successCode=recv(clientSocketDescriptor,response,sizeof(response),0);
if(successCode>0)
{
printf("Response recived\n");
printf("%s\n",response);
}
closesocket(clientSocketDescriptor);
WSACleanup( );
return 0;
}