#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>
#include<ufwp>
#include<iostream>
using namespace std;
using namespace ufwp;
typedef struct _request
{
char *method;
char *resource;
char isClientSideTechnologyResource;
char *mimeType;
int dataCount;
char **data;
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
left++;
right++;
}
return *left==*right;
}

char* getMimeType(char *resource)
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
if(extensionEquals(resource+lastDotIndex+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+lastDotIndex+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastDotIndex+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastDotIndex+1,"bmp"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/bmp");
}
if(extensionEquals(resource+lastDotIndex+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}

char isClientResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0')return 'N';
return 'Y';
}

void serverInfo(int &dataCount,char** &data,char *bytes)
{
dataCount=0;
data=new char*[10];
int e=0,f=0;
char *temp;
while(bytes[e]!=' ')
{
while(bytes[e]!='=')e++;
e++;
f=0;
temp=new char[50];
while(bytes[e]!='&' && bytes[e]!=' ')
{
temp[f]=bytes[e];
e++;
f++;
}
temp[f]='\0';
data[dataCount]=temp;
dataCount++;
}
}

REQUEST * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int dataCount=0;
char **data=NULL;
int i,j;
for(i=0;bytes[i]!=' ';i++)method[i]=bytes[i];
method[i]='\0';
i+=2;
for(j=0;bytes[i]!=' ' && bytes[i]!='?';i++,j++)resource[j]=bytes[i];
if(bytes[i]=='?')serverInfo(dataCount,data,&bytes[i]);
resource[j]='\0';
printf("Request arrived for (%s)\n",resource);
REQUEST *request=(REQUEST *)malloc(sizeof(REQUEST));
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
request->dataCount=dataCount;
request->data=data;
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientResource(resource);
request->mimeType=getMimeType(resource);
request->dataCount=dataCount;
request->data=data;
}
return request;
}

UfWebProjector::UfWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->url=NULL;
this->ptrOnRequest=NULL;
}

void UfWebProjector::onRequest(const char *url,void(*ptrOnRequest)(int,char **))
{
if(this->url)delete [ ] this->url;
this->url=NULL;
this->ptrOnRequest=NULL;
if(url==NULL || ptrOnRequest==NULL)return;
this->url=new char[strlen(url)+1];
strcpy(this->url,url);
this->ptrOnRequest=ptrOnRequest;
}

UfWebProjector::~UfWebProjector( )
{
if(this->url)delete [ ] this->url;
}

void UfWebProjector::start( )
{
WORD ver;
WSADATA wsaData;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
char responseBuffer[1025];
char requestBuffer[8192];
int len;
int bytesExtracted;
int successCode;

FILE *f;
int length;
int i,rc;
char g;

ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
WSACleanup( );
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind the server\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return ;
}
listen(serverSocketDescriptor,10);
while(1)
{
//loop starts here.

printf("MY SERVER IS READY TO ACCEPT REQUEST\n");
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to connect to client side\n");
closesocket(serverSocketDescriptor);
WSACleanup( );
return ;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(bytesExtracted<0)
{
//what to do is yet to be decided
}
else
{
if(bytesExtracted==0)
{
//what to do is yet to be decide
}
else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f!=NULL)printf("Sending Index.html\n");
 if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL)printf("Sending Index.htm\n");
if(f==NULL)
{
printf("Sending 404 page\n");
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : 163\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
//open index.htm

fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nKeep-Alive : timeout=5 , max=1000\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);

i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}
else
{
//open index.html
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nKeep-Alive : timeout=5 , max=1000\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);

i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}
else
{
//look for request->resource
f=fopen(request->resource,"rb");
/*mera code.
if(strcmp(request->resource,this->serverResource)==0)
{
this->funPtr( );
}
mera code end.*/
if(f!=NULL)printf("Sending %s\n",request->resource);
if(f==NULL)
{
//resource not found
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
//server resource
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : %s\nContent-Length : %d\nKeep-Alive : timeout=5 , max=1000\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);

i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}

}
}
else
{
//more changes required in context to server side resource.
if(this->url==NULL || this->ptrOnRequest==NULL)
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
int ii=0;
if(this->url[ii]=='/')ii++;
if(strcmp(request->resource,this->url+ii)==0)
{
this->ptrOnRequest(request->dataCount,request->data);
printf("Sending processed page");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : green'>Resource /%s processed</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
}

}
}
}


}   //this is the ending paranthasis of the infinte loop.


closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return ;
}