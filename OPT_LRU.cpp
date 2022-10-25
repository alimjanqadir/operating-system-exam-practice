 

这是我今天作的，因为还有时间我一起把lru算法也作了

#include<stdio.h>
#include<iostream.h>
#include<string.h>

const int maxpage=100;
const int maxmem=3;

typedef struct node
{
int pageno;
int pageflag;
}PAGE;
PAGE page[maxmem];
int pagenum[maxpage];
int m;
///////////////////////////////////
void init()
{
int i;
for (i=0;i<maxmem;i++)
{
  page[i].pageno=0;
  page[i].pageflag=0;
}

return;
}
//////////////////////////////////
int readata()
{
char filename[20];
FILE * fp;
int i=0;
cout<<"请输入页面号文件名:";
    strcpy(filename,"hhg.txt");
//cin>>filename;
if ((fp=fopen(filename,"r"))==NULL)
{
  cout<<"文件没有打开！";
  return 0;
}
while (!feof(fp))
{
  fscanf(fp,"%d,",&pagenum[i]);
  i++;
}
m=i;
cout<<endl;
cout<<"读入的页面号为:"<<endl;
    for(i=0;i<m;i++)
{
  cout<<pagenum[i]<<"  ";
}
cout<<endl;
fclose(fp);
return 1;
}
///////////////////////////////////
void pageopt()
{
int i,k,kk,aa=100,bb=100,cc=100;
cout<<"最理想算法："<<endl;
for(i=0;i<3;i++)
{
  page[i].pageno=pagenum[i];
  cout<<"x  ";
}
//-------------
for(i=3;i<m;i++)
{
  aa=100;bb=100;cc=100;
  if (page[0].pageno!=pagenum[i]&&page[1].pageno!=pagenum[i]&&page[2].pageno!=pagenum[i])
  {
    for(k=i;k<m;k++)
    {
      if (page[0].pageno==pagenum[k])   aa=k;
      
    }
    for(k=i;k<m;k++)
    {
      if (page[1].pageno==pagenum[k]) bb=k;
    }
    for(k=i;k<m;k++)
    {
      if (page[2].pageno==pagenum[k]) cc=k;
      
    }
    
   if (aa>bb)
   {
    if(aa>cc) kk=0;
   }
   else
   {   
    if (bb>cc) kk=1;
               else kk=2;
   } 
   cout<<page[kk].pageno<<"  ";
      page[kk].pageno=pagenum[i];
  }
   else
   {
  cout<<"*  ";
   }
}
cout<<endl;
}
///////////////////////////////////
void lru()
{
int i,j,k,temp,flaga;
cout<<"LRU算法："<<endl;
for(i=0;i<3;i++)
{
  page[i].pageno=pagenum[i];
  cout<<"x  ";
}
  for(i=3;i<m;i++)
  {
   if (page[0].pageno!=pagenum[i]&&page[1].pageno!=pagenum[i]&&page[2].pageno!=pagenum[i])
   {
    cout<<page[0].pageno<<"  ";
    for(j=0;j<2;j++)
     page[j].pageno=page[j+1].pageno;
    page[2].pageno=pagenum[i];
    
   }
   else 
   {for(j=0;j<3;j++)
    if (page[j].pageno==pagenum[i]) flaga=j;

    temp=page[flaga].pageno;
    for(k=flaga;k<2;k++)
     page[k].pageno=page[k+1].pageno;

    page[2].pageno=temp;
    cout<<"*  ";
   }
  
  }
cout<<endl<<endl;
}
void main()
{
int f;
init();
f=readata();
if (f)
{
  pageopt();
  cout<<endl;
  lru();
  cout<<endl;
  cout<<endl;
  cout<<endl;
}
}



 
