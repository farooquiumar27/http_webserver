#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
void findAndReplace(const char *resource)
{
cout<<resource<<endl;
string p=to_string(95);
string c=to_string(85);
string m=to_string(93);
string h=to_string(82);
string e=to_string(87);
string s1="${physics}";
string s2="${chemistry}";
string s3="${maths}";
string s4="${hindi}";
string s5="${english}";

FILE *f,*t;
int pos;
f=fopen(resource,"r");
t=fopen("temp.xyz","a");
char a,b;
while(1)
{
a=fgetc(f);
if(feof(f))break;
if(a=='$')
{
string arr;
pos=ftell(f);
arr=arr+"$";
while(arr[arr.size( )-1]!='}')
{
b=fgetc(f);
if(feof(f))break;
arr=arr+b;
}

if(feof(f))for(int i=0;i<arr.size( );i++)fputc(arr[i],t);  //file ended before reading '}'.
else if(strncmp(arr.c_str( ),"${physics}",10)==0)for(int i=0;i<p.size( );i++)fputc(p[i],t);
else if(strncmp(arr.c_str( ),"${chemistry}",12)==0)for(int i=0;i<c.size( );i++)fputc(c[i],t);
else if(strncmp(arr.c_str( ),"${maths}",8)==0)for(int i=0;i<m.size( );i++)fputc(m[i],t);
else if(strncmp(arr.c_str( ),"${hindi}",8)==0)for(int i=0;i<h.size( );i++)fputc(h[i],t);
else if(strncmp(arr.c_str( ),"${english}",10)==0)for(int i=0;i<e.size( );i++)fputc(e[i],t);
else
{
fputc(a,t);
fseek(f,pos,SEEK_SET);
}
}
else
{
fputc(a,t);
}
}
fclose(f);
fclose(t);
f=fopen(resource,"w");
t=fopen("temp.xyz","r");
while(1)
{
a=fgetc(t);
if(feof(t))break;
fputc(a,f);
}
fclose(f);
fclose(t);
t=fopen("temp.xyz","w");
fclose(t);
}
int main( )
{
string str="/abcd.xyz";
findAndReplace(&str[1]);
return 0;
}