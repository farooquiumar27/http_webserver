#include<ufwp>
#include<iostream>
#include<string.h>
using namespace ufwp;
using namespace std;
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
void sendStudentEditForm(Request &request,Response &response)
{
string roll=request.get("rl");
string name;
FILE *f;
int found=0;
f=fopen("info.data","rb");
dataWrapper data;
while(1)
{
fread(&data,sizeof(dataWrapper),1,f);
if(feof(f))break;
if(data.rollNo==roll)
{
found=1;
name=data.name;
break;
}
}
if(found==1)
{
response.write("<! DOCTYPE HTML>\n");
response.write("<html lang='en'>\n");
response.write("<head>\n");
response.write("<meta charset='utf-8'>\n");
response.write("<title>ABCL School , Ujjain</title>\n");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)\n");
response.write("<form method='get' action='updateStudent'>\n");
response.write("Roll number : ");
response.write(roll.c_str( ));
response.write("\n<input type='hidden' id='rl' name='rl' value='");
response.write(roll.c_str( ));
response.write("'><br>\n");
response.write("Name<input type='text' id='nm' name='nm' value='");
response.write(name.c_str( ));
response.write("'><br>\n");
response.write("<button type='submit'>Update</button>\n");
response.write("</form>\n");
response.write("<a href='index.html'>Home</a>\n");
response.write("</body>\n");
response.write("</html>\n");
response.sendResponse( );
response.close( );
}

}
void updateStudentData(Request &request,Response &response)
{
string roll;
string name;
roll=request.get("rl");
name=request.get("nm");
FILE *f,*g;
dataWrapper data;
f=fopen("info.data","rb");
g=fopen("tmp.data","ab");
while(1)
{
fread(&data,sizeof(dataWrapper),1,f);
if(feof(f))break;
if(strcmp(data.rollNo,roll.c_str( ))!=0)
{
fwrite(&data,sizeof(dataWrapper),1,g);
}
else
{
strcpy(data.name,name.c_str( ));
fwrite(&data,sizeof(dataWrapper),1,g);
}
}
fclose(f);
fclose(g);
f=fopen("info.data","wb");
g=fopen("tmp.data","rb");
while(1)
{
fread(&data,sizeof(dataWrapper),1,g);
if(feof(g))break;
fwrite(&data,sizeof(dataWrapper),1,f);
}
fclose(f);
fclose(g);
g=fopen("tmp.data","wb");
fclose(g);
response.write("<! DOCTYPE HTML>\n");
response.write("<html lang='en'>\n");
response.write("<head>\n");
response.write("<meta charset='utf-8'>\n");
response.write("<title>ABCL School , Ujjain</title>\n");
response.write("</head>");
response.write("<body>");
response.write("Student (Edit Module)<br>\n");
response.write("<h3>Student update</h3>\n");
response.write("<form method='get' action='index.html'>\n");
response.write("<button type='submit'>OK</button>\n");
response.write("</form>\n");
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
server.onRequest("/editStudent",sendStudentEditForm);
server.onRequest("/updateStudent",updateStudentData);
server.start( );
return 0;
}