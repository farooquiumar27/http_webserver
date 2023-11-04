#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>

typedef struct _request
{
char *method;
char *resource;
char isClientSideTechnologyResource;
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
return mimeType;
}

char isClientResource(char *resource)
{
return 'Y';   //this will have to be changed later on
}

REQUEST * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++)method[i]=bytes[i];
method[i]='\0';
i+=2;
for(j=0;bytes[i]!=' ';i++,j++)resource[j]=bytes[i];
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
}
else
{
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource='Y';
request->mimeType=getMimeType(resource);
}
return request;
}

int main( )
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
int i;
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
return 0;
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
while(1)
{
g=getc(f);
if(feof(f))
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;
}
responseBuffer[i++]=g;
if(i==1024)
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
}
}
fclose(f);

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
while(1)
{
g=getc(f);
if(feof(f))
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;
}
responseBuffer[i++]=g;
if(i==1024)
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
}
}
fclose(f);

}
}
else
{
//look for request->resource
f=fopen(request->resource,"rb");
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
while(1)
{
g=getc(f);
if(feof(f))
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;
}
responseBuffer[i++]=g;
if(i==1024)
{
responseBuffer[i]='\0';
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
}
}
fclose(f);

}

}
}
else
{
//what to do in case of server side technology is yet to be decided
}
}
}


}   //this is the ending paranthasis of the infinte loop.


closesocket(clientSocketDescriptor);
closesocket(serverSocketDescriptor);
WSACleanup( );
return 0;
}