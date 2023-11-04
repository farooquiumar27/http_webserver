#include<ufwp>
#include<iostream>
#include<string.h>
using namespace ufwp;
using namespace std;
/*
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
*/
typedef struct da
{
public:
char rollNo[51];
char name[51];
}dataWrapper;

void addStud(Request &request,Response &response)
{
char rollNo[51],name[51];
string roll,nm;
roll=request.get("rl");
nm=request.get("nm");
dataWrapper data;
strcpy(data.rollNo,roll.c_str( ));
strcpy(data.name,nm.c_str( ));
printf("%s\n",data.rollNo);
printf("%s\n",data.name);
FILE *f;
f=fopen("info.data","ab");
fwrite(&data,sizeof(data),1,f);
fclose(f);
response.write("<! DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<meta charset='utf-8'>");
response.write("<head>");
response.write("<title>ABCL School , Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("Student Added");
response.write("<BR>");
response.write("<a href='index.html'>Home</a>\n");
response.write("</body>");
response.write("</html>");
response.sendResponse( );
response.close( );
}


void getAllStudents(Request &request,Response &response)
{
FILE *f;
f=fopen("info.data","rb");
dataWrapper data;
response.write("<! DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<meta charset='utf-8'>");
response.write("<head>");
response.write("<title>ABCL School , Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("<Table , border='1'>");
response.write("<TR><TD>S.No</td><td>Roll.No</td><td>Name</td></TR>");
int e=1;
char str[4];
while(1)
{
fread(&data,sizeof(dataWrapper),1,f);
if(feof(f))break;
response.write("<TR>\n");
response.write("<TD>");
sprintf(str,"%d",e);
response.write(str);
response.write("</TD>\n");
response.write("<TD>");
response.write(data.rollNo);
response.write("</TD>\n");
response.write("<TD>");
response.write(data.name);
response.write("</TD>\n");
response.write("</TR>");
e++;
}
response.write("</Table>\n");
response.write("<a href='index.html'>Home</a>\n");
fclose(f);
response.write("</body>\n");
response.write("</html>\n");
response.sendResponse( );
response.close( );
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.start( );
return 0;
}