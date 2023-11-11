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
response.write("Student (Add Module)<br>\n");
response.write("<h3>Student Added</h3>\n");
response.write("<form method='get' action='index.html'>\n");
response.write("<button type='submit'>OK</button>\n");
response.write("</form>\n");
response.write("</body>");
response.write("</html>");
response.sendResponse("text/html");
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
response.sendResponse("text/html");
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
response.sendResponse("text/html");
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
response.sendResponse("text/html");
response.close( );
}

void sendStudentDeleteConfirmationForm(Request &request,Response &response)
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
response.write("Student (Delete Module)\n");
response.write("<form method='get' action='deleteStudent'>\n");
response.write("Roll number : ");
response.write(roll.c_str( ));
response.write("<br>");
response.write("Name : ");
response.write(name.c_str( ));
response.write("\n<input type='hidden' id='rl' name='rl' value='");
response.write(roll.c_str( ));
response.write("'><br>\n");
response.write("<button type='submit'>Delete</button>\n");
response.write("</form>\n");
response.write("<a href='index.html'>Home</a>\n");
response.write("</body>\n");
response.write("</html>\n");
response.sendResponse("text/html");
response.close( );
}

}
void deleteStudentData(Request &request,Response &response)
{
string roll;
roll=request.get("rl");
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
response.write("Student (Delete Module)<br>\n");
response.write("<h3>Student Deleted</h3>\n");
response.write("<form method='get' action='index.html'>\n");
response.write("<button type='submit'>OK</button>\n");
response.write("</form>\n");
response.write("</body>\n");
response.write("</html>\n");
response.sendResponse("text/html");
response.close( );
}
void functionOne(Request &request,Response &response)
{
cout<<"One got called"<<endl;
request.forward("/xyz.html");
}
void functionTwo(Request &request,Response &response)
{
cout<<"Two got called"<<endl;
request.forward("/cool.jpg");
}
void functionThree(Request &request,Response &response)
{
cout<<"Three got called"<<endl;
request.setInt("abcd",23);
request.setInt("pqr",33);
request.setInt("lmn",96);
request.forward("/functionFour");
}
void functionFour(Request &request,Response &response)
{
cout<<"Four got called"<<endl;
int num1,num2,num3;
num1=request.getInt("abcd");
num2=request.getInt("pqr");
num3=request.getInt("lmn");
cout<<"Num _1 -> "<<num1<<endl;
cout<<"Num_2 -> "<<num2<<endl;
cout<<"Num_3 -> "<<num3<<endl;
response.write("<! DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<meta charset='utf-8'>");
response.write("<head>");
response.write("<title>ABCL School , Ujjain</title>");
response.write("</head>");
response.write("<body>");
response.write("ABCL is a shining star");
response.write("<BR>");
response.write("Num_1 -> ");
response.write(to_string(num1).c_str( ));
response.write("<BR>");
response.write("Num_2 -> ");
response.write(to_string(num2).c_str( ));
response.write("<BR>");
response.write("Num_3 -> ");
response.write(to_string(num3).c_str( ));
response.write("<BR>");
response.write("<a href='index.html'>Home</a>\n");
response.write("</body>");
response.write("</html>");
response.sendResponse("text/html");
response.close( );
}
void sendStudentMarksheet(Request &request,Response &response)
{
int rl=atoi(request.get("rl").c_str( ));
int p=0,c=0,m=0,h=0,e=0,b=0;
if(rl==101)
{
p=90;
c=85;
m=97;
h=95;
e=90;
b=40;
}
if(rl==102)
{
p=80;
c=75;
m=90;
h=91;
e=85;
b=47;
}
if(rl==103)
{
p=70;
c=65;
m=87;
h=80;
e=75;
b=39;
}
if(rl==104)
{
p=50;
c=45;
m=47;
h=65;
e=70;
b=23;
}
if(rl==105)
{
p=95;
c=95;
m=97;
h=95;
e=95;
b=80;
}
request.setInt("physics",p);
request.setInt("chemistry",c);
request.setInt("maths",m);
request.setInt("hindi",h);
request.setInt("english",e);
request.setInt("biolodgy",b);
request.forward("/Marksheet.sct");
}
int main( )
{
UfWebProjector server(8080);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",sendStudentEditForm);
server.onRequest("/updateStudent",updateStudentData);
server.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmationForm);
server.onRequest("/deleteStudent",deleteStudentData);
server.onRequest("/functionOne",functionOne);
server.onRequest("/functionTwo",functionTwo);
server.onRequest("/functionThree",functionThree);
server.onRequest("/functionFour",functionFour);
server.onRequest("/getStudentMarksheet",sendStudentMarksheet);
server.start( );
return 0;
}