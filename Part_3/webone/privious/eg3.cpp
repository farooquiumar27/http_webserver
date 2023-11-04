#include<ufwp>
#include<iostream>
using namespace ufwp;
using namespace std;
void doSomething(int dataCount,char **data)
{
char *temp;
for(int e=0;e<dataCount;e++)
{
temp=data[e];
printf("%s\n",temp);
}
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/register",doSomething);
server.start( );
return 0;
}