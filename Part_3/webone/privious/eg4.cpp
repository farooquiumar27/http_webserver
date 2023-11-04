#include<ufwp>
#include<iostream>
using namespace ufwp;
using namespace std;
/* OLD VERSION
void doSomething(int dataCount,char **data)
{
char *temp;
for(int e=0;e<dataCount;e++)
{
temp=data[e];
printf("%s\n",temp);
}
}
*/
//NOW NEW VERSION
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
response.write("Hello");
response.write(n.c_str( ));
response.write("<BR>");
response.write("Data Saved");
response.write("</body>");
response.write("</html>");
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/register",doSomething);
server.start( );
return 0;
}