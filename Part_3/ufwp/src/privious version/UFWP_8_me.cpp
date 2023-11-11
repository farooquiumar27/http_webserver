#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>
#include<ufwp>
#include<iostream>
#include<map>
#include<utility>
using namespace std;
using namespace ufwp;

void URLEncoder::decode(int dataCount,char ** &data)
{
for(int e=0;e<dataCount;e++)
{
for(int f=0;data[e][f]!='\0';f++)
{
if(data[e][f]=='+')data[e][f]=' ';
}
}
}
void URLEncoder::encode(int dataCount,char ** &data)
{
for(int e=0;e<dataCount;e++)
{
for(int f=0;data[e][f]!='\0';f++)
{
if(data[e][f]==' ')data[e][f]='+';
}
}
}


Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
this->FILE_NAME="response.data";
this->clear( );
}
void Response::write(const char *_response)
{
FILE *f=fopen(FILE_NAME.c_str( ),"ab");
fwrite(_response,strlen(_response),1,f);
fclose(f);
}
void Response::write(const char *_response,int length)
{
FILE *f=fopen(FILE_NAME.c_str( ),"ab");
fwrite(_response,length,1,f);
fclose(f);
}
int Response::size( )
{
FILE *f;
f=fopen(FILE_NAME.c_str( ),"rb");
fseek(f,0,SEEK_END);
int length=ftell(f);
fseek(f,0,SEEK_SET);
fclose(f);
return length;
}
void Response::sendResponse(const char *mimeType)
{
int length,i,rc;
char responseBuffer[1024];
FILE *f;
f=fopen(FILE_NAME.c_str( ),"rb");
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : %s\nContent-Length : %d\nConnection: close\n\n",mimeType,length);
send(this->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(this->clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
this->clear( );
}
void Response::clear( )
{
FILE *f;
f=fopen(this->FILE_NAME.c_str( ),"wb");
fclose(f);
}
void Response::close( )
{
closesocket(this->clientSocketDescriptor);
}

Request::Request(char *bytes)
{
this->request=parseRequest(bytes);
this->response=NULL;
}
Request::~Request( )
{
this->clear( );
}
void Request::setResponse(Response *response)
{
this->response=response;
}
string Request::get(const char *info)
{
int len;
int found=0;
string _string;
for(int e=0;e<this->request->dataCount;e++)
{
len=0;
for(int f=0;this->request->data[e][f]!='=';f++)len++;
if(strncmp(info,this->request->data[e],len)==0)
{
found=1;
_string=&(this->request->data[e][len+1]);
break;
}
}
if(found==0)return string("Not found");
else return _string;
}
void Request::clear( )
{
if(this->request!=NULL)
{
free(this->request->method);
if(this->request->resource!=NULL)
{
free(this->request->resource);
if(this->request->mimeType!=NULL)free(this->request->mimeType);
}
if(this->request->data!=NULL)
{
for(int k=0;k<this->request->dataCount;k++)free(this->request->data[k]);
free(this->request->data);
}
//if(this->response!=NULL)free(this->response);  //point of dout 
free(this->request);
}
}
const char* Request::giveResource( )
{
return this->request->resource;
}
char Request::giveTechnologySide( )
{
return this->request->isClientSideTechnologyResource;
}
const char* Request::giveMime( )
{
return this->request->mimeType;
}
void Request::fillMap(string url,void(*ptr)(Request &,Response &))
{
if(url.c_str( )==NULL || ptr==NULL)return;
this->serverSideTechnologyRequestMap_2.insert(pair<string,void(*)(Request &,Response &)>(url,ptr));
}
void Request::forward(const char *resource)
{
if(resource==NULL)return;
cout<<"Request forwarded for "<<resource<<endl;
char responseBuffer[1024];
FILE *f;
int length,rc,i;
int ii=0;
if(resource[ii]=='/')ii++;
char *_resource=(char *)malloc(sizeof(char)*strlen(resource)+1);
strcpy(_resource,resource);
_resource[strlen(_resource)]='\0';
if(isClientResource(_resource)=='Y')
{
f=fopen(&resource[ii],"rb");
if(f==NULL)
{
//resource not found
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",resource);
this->response->write(temp);
this->response->sendResponse("text/html");
this->response->close( );
this->response->clear( );
this->clear( );
}
else
{
//client technology resource
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
//sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : %s\nContent-Length : %d\nConnection: close\n\n",getMimeType(resourse),length);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
//send(clientSocketDescriptor,responseBuffer,rc,0);
this->response->write(responseBuffer,rc);
i+=rc;
}
fclose(f);
this->response->sendResponse(this->getMimeType(_resource));
this->response->close( );
this->response->clear( );
this->clear( );
}
}
else
{
map<string,void(*)(Request &,Response &)>::iterator it;
it=this->serverSideTechnologyRequestMap_2.find(string(&resource[ii]));
int fou=(it!=this->serverSideTechnologyRequestMap_2.end( ))?1:0;
if(fou==0)
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",&resource[ii]);
this->response->write(temp);
this->response->sendResponse("text/html");
this->response->close( );
this->response->clear( );
this->clear( );
}
else
{
void(*ptr)(Request &,Response &);
ptr=it->second;
ptr(*this,*(this->response));
this->response->close( );
this->response->clear( );
this->clear( );
}
}
free(_resource);
}
int Request::extensionEquals(const char *left,const char *right)
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

char* Request::getMimeType(char *resource)
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

char Request::isClientResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0')return 'N';
return 'Y';
}

Request::REQUEST * Request::parseRequest(char *bytes)
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
URLEncoder::decode(request->dataCount,request->data);
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
//this->url=NULL;
//this->ptrOnRequest=NULL;
}

void UfWebProjector::onRequest(const char *url,void(*ptrOnRequest)(Request &,Response &))
{
if(url==NULL || ptrOnRequest==NULL)return;
int e=0;
if(url[e]=='/')e++;
string temp(&url[e]);
this->serverSideTechnologyRequestMap.insert(pair<string,void(*)(Request &,Response &)>(temp,ptrOnRequest));
}

UfWebProjector::~UfWebProjector( )
{
//if(this->url)delete [ ] this->url;
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
//REQUEST *request=parseRequest(requestBuffer);

Request _Request(requestBuffer);

if(this->serverSideTechnologyRequestMap.size( )>0)
{
for(map<string,void(*)(Request &,Response &)>::iterator it=this->serverSideTechnologyRequestMap.begin( );it!=this->serverSideTechnologyRequestMap.end( );++it)
{
_Request.fillMap(it->first,it->second);
}
}

if(_Request.giveTechnologySide( )=='Y')
{
if(_Request.giveResource( )==NULL)
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
_Request.clear( );
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
_Request.clear( );
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
_Request.clear( );
fclose(f);
closesocket(clientSocketDescriptor);
}
}
else
{
//look for request->resource
f=fopen(_Request.giveResource( ),"rb");

if(f!=NULL)printf("Sending %s\n",_Request.giveResource( ));
if(f==NULL)
{
//resource not found
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",_Request.giveResource( ));
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
_Request.clear( );
}
else
{
//server resource
fseek(f,0,SEEK_END); //move the internal pointer to the end of the file.
length=ftell(f);
fseek(f,0,SEEK_SET); //move the internal pointer to the start of file.
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : %s\nContent-Length : %d\nConnection: close\n\n",_Request.giveMime( ),length);
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
_Request.clear( );
fclose(f);
closesocket(clientSocketDescriptor);
}

}
}
else
{
//more changes required in context to server side resource.
if(this->serverSideTechnologyRequestMap.size( )==0)
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",_Request.giveResource( ));
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
_Request.clear( );
}
else
{
int ii=0;
map<string,void(*)(Request &,Response &)>::iterator i;
string _temp(_Request.giveResource( ));
i=serverSideTechnologyRequestMap.find(_temp);
if(i!=serverSideTechnologyRequestMap.end( ))
{
Response _response(clientSocketDescriptor);
_Request.setResponse(&_response);
void(*ptr)(Request &,Response &);
ptr=i->second;
ptr(_Request,_response);
_Request.clear( );
}
else
{
printf("Sending 404 page\n");
char temp[501];
sprintf(temp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>UF WEB PROJECTOR</title></head><body><h2 style='color : red'>Resource /%s not found</h2></body></html>",_Request.giveResource( ));
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type : text/html\nContent-Length : %d\nConnection: close\n\n",strlen(temp));
strcat(responseBuffer,temp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
_Request.clear( );
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

