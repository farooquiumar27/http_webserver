#include<ufwp>
#include<iostream>
using namespace ufwp;
using namespace std;
void doSomething( )
{
cout<<"This code got excetued on server side"<<endl;
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/kkk",doSomething);
server.start( );
return 0;
}