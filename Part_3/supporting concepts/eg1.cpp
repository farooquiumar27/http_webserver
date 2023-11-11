#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

void findAndReplace(int x,int y)
{
string num1=to_string(x);
string num2=to_string(y);
FILE *f,*t;
int pos;
f=fopen("abcd.xyz","r");
t=fopen("temp.xyz","a");
char arr[4];
char a,b;
while(1)
{
a=fgetc(f);
if(feof(f))break;
if(a=='$')
{
pos=ftell(f);
arr[0]='$';
arr[1]='\0';
arr[2]='\0';
arr[3]='\0';
for(int i=1;i<=3;i++)
{
b=fgetc(f);
if(feof(f))break;
arr[i]=b;
}
if(arr[3]=='\0')for(int i=0;arr[i]!='\0';i++)fputc(arr[i],t);  //file ended before readline the 4 characters.
else if(strncmp(arr,"${x}",4)==0)for(int i=0;i<num1.size( );i++)fputc(num1[i],t);    //the 4 readed char matches with ${x}
else if(strncmp(arr,"${y}",4)==0)for(int i=0;i<num2.size( );i++)fputc(num2[i],t);    //the 4 readed char matches with ${y}
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

f=fopen("abcd.xyz","w");
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
int x,y;
cout<<"Enter the value of x : ";
cin>>x;
cout<<"Enter the value of y : ";
cin>>y;
findAndReplace(x,y);
return 0;
}