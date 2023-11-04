#include<ufwp>
#include<iostream>
using namespace ufwp;
using namespace std;
void doSomething(Request &request,Response &response)
{
string n=request.get("nm");
string c=request.get("ct");
response.write("<! DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<meta charset='utf-8'>");
response.write("<head>");
response.write("<title>GAME</title>");
response.write("</head>");
response.write("<body>");
response.write("Hello ");
response.write(n.c_str( ));
response.write("<BR>");
response.write("Data Saved");
response.write("</body>");
response.write("</html>");
response.sendResponse( );
response.close( );
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/register",doSomething);
server.start( );
return 0;
}