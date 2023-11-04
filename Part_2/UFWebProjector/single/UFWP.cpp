#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

typedef struct _request
{
char *method;
char *resource;
char isClientSideResourceTechnology;
char *mimeType;
}REQUEST;

int extensionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90)a=a+32;
if(b>=65 && b<=90)b=b+32;
if(a!=b)return 0;
}
return *left==*right;
}

char * getMimeType(char *resource)
{
char *mimeType;
mimeType=NULL;
int len=strlen(resource);
if(len<4)return mimeType;
int lastDotIndex=len-1;
while(lastDotIndex>0 && resource[lastDotIndex]!='.')lastDotIndex--;
if(lastDotIndex==0)return mimeType;
if(extensionEquals(resource+lastDotIndex+1,"html"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+lastDotIndex+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+lastDotIndex+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javacript");
}
return mimeType;
}

char isClientResource(char *resource)
{
return 'Y';   //this will have to be changed later on
}

REQUEST	 * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++)method[i]=bytes[i];
method[i]='\0';
for(j=0;bytes[i]!=' ';i++,j++)resource[j]=bytes[i];
resource[j]='\0';
REQUEST *request;
request=(REQUEST *)malloc(sizeof(REQUEST));
int tmp=strlen(method)+1;
request->method=(char *)malloc(sizeof(char)*tmp);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideResourceTechnology='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideResourceTechnology=isClientResource( resource);
request->mimeType=getMimeType(resource);
}
return request;
}

int main( )
{
WSADATA wsaData;
WORD ver;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int successCode;
int bytesExtracted;
int len;
char requestBuffer[8192];     //8KB
char responseBuffer[1025];   //1024  + 1 for ['\0']
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
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(bytesExtracted<0)
{
//what to do yet to be decided.
}
else
{
if(bytesExtracted==0)
{
//what to do yet to be decided.
}
else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
if(request->isClientSideResourceTechnology!='Y')
{
//what to do in case of server side technology is yet to be decided.
}
else
{
/*
if request->response = NULL look for index.html and serve
else look for index.htm and serve 
else send back 404
*/
}
}
}
strcpy(responseBuffer,"WELCOME TO OF MY WORLD");
successCode=send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
if(successCode<0)
{
printf("Unable to send responseBuffer\n");
}
closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}