#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>
#include<ufwp>
#include<iostream>
using namespace std;
using namespace ufwp;
class URLEncoder
{
public:
static void decode(int dataCount,char ** &data)
{
for(int e=0;e<dataCount;e++)
{
for(int f=0;data[e][f]!='\0';f++)
{
if(data[e][f]=='+')data[e][f]=' ';
}
}
}
static void encode(int dataCount,char ** &data)
{
for(int e=0;e<dataCount;e++)
{
for(int f=0;data[e][f]!='\0';f++)
{
if(data[e][f]==' ')data[e][f]='+';
}
}
}
};

Request::Request(int dataCount,char **data)
{
if(dataCount<1 || data==NULL)return;
this->data=(char **)malloc(sizeof(char *)*dataCount);
this->dataCount=dataCount;
for(int e=0;e<dataCount;e++)
{
this->data[e]=(char *)malloc(sizeof(char)*(strlen(data[e])+1));
strcpy(this->data[e],data[e]);
this->data[e][strlen(data[e])]='\0';
}
}
Request::~Request( )
{
for(int e=0;e<this->dataCount;e++)free(data[e]);
free(data);
}
string Request::get(const char *info)
{
int len;
int found=0;
string _string;
for(int e=0;e<this->dataCount;e++)
{
len=0;
for(int f=0;this->data[e][f]!='=';f++)len++;
if(strncmp(info,this->data[e],len)==0)
{
found=1;
_string=&data[e][len+1];
break;
}
}
if(found==0)return string("Not found");
else return _string;
}

void Response::write(const char *_response)
{
if(_response!=NULL)this->response=this->response+_response;
}
int Response::size( )
{
return this->response.size( );
}
const char * Response::c_str( )
{
return this->response.c_str( );
}

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
if(strcmp(method,"GET")==0)
{
for(j=0;bytes[i]!=' ';i++,j++)
{
if(bytes[i]=='?')break;
resource[j]=bytes[i];
}
resource[j]='\0';
i++;
int si=i;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')dataCount++;
i++;
}
dataCount++;
data=(char **)malloc(sizeof(char *)*dataCount);
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}

if(pc!=NULL)
{
free(pc);
}

}
printf("Request arrived for (%s)\n",resource);
REQUEST *request=(REQUEST *)malloc(sizeof(REQUEST));
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
request->dataCount=dataCount;
request->data=data;
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
request->isClientSideTechnologyResource=isClientResource(resource);
request->mimeType=getMimeType(resource);
}
return request;
}

UfWebProjector::UfWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->url=NULL;
this->ptrOnRequest=NULL;
}

void UfWebProjector::onRequest(const char *url,void(*ptrOnRequest)(Request &,Response &))
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
printf("Control [1] IN\n");
}
else
{
if(bytesExtracted==0)
{
//what to do is yet to be decide
printf("Control [2] IN\n");
}
else
{
printf("Control [3] IN\n");
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
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : 163\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource / not found</h2></body></html>");
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
//open index.htm

fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",length);
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
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
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
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",length);
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
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
fclose(f);
closesocket(clientSocketDescriptor);
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
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
}
else
{
//server resource
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : %s\nContent-Length : %d\nConnection: close\n\n",request->mimeType,length);
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
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
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
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
}
else
{
int ii=0;
if(this->url[ii]=='/')ii++;
if(strcmp(request->resource,this->url+ii)==0)
{
URLEncoder::decode(request->dataCount,request->data);
Request _request(request->dataCount,request->data);
Response _response;
this->ptrOnRequest(_request,_response);

if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++)free(request->data[k]);
free(request->data);
}

printf("Sending processed page");
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",_response.size( ));
strcat(responseBuffer,_response.c_str( ));
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
}
else
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
if(request!=NULL)
{
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
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

