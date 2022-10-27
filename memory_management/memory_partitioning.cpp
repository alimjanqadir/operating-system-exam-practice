/*可变分区存储管理方案中的内存分配

  可变分区调度算法有:最先适应分配算法,最优适应分配算法,最坏适应算法
用户提出内存空间的申请；系统根据申请者的要求，按照一定的分配策略分析内存空间的使用情况，找出能
满足请求的空闲区，分给申请者；当程序执行完毕或主动归还内存资源时，系统要收回它所占用的内存空间
或它归还的部分内存空间。

1．程序运行时首先接收输入：空闲区数据文件，包括若干行，每行有两个数据项：起始地址、长度（均为
整数），各数据项以逗号隔开。

2．建立空闲区表并在屏幕上显示输出空闲区表内容，空闲区表中记录了内存中可供分配的空闲区的始址和
长度，用标志位指出该分区是否是未分配的空闲区。

3．从用户界面根据用户提示接收一个内存申请，格式为：作业名、申请空间的大小。

4．按照最差（最坏）适配算法选择一个空闲区，分割并分配，修改相应的数据结构（空闲区表），填写内
存已分配区表（起始地址、长度、标志位），其中标志位的一个作用是指出该区域分配给哪个作业。

5．重复3、4，直到输入为特殊字符（0）。

6．在屏幕上显示输出新的空闲区表和已分配区表的内容。

本程序包括:FIFO,最优适应分配算法,最坏适应算法

  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include <iostream>

using namespace std;

const int MAXJOB = 100; //定义表最大记录数

typedef struct node
{
  int start;
  int length;
  char tag[20];
} job;

job frees[MAXJOB]; //定义空闲区表
int free_quantity;

job occupys[MAXJOB]; //定义已分配区表
int occupy_quantity;

//初始化函数
void initial()
{
  int i;

  for (i = 0; i < MAXJOB; i++)
  {
    frees[i].start = -1;
    frees[i].length = 0;
    strcpy(frees[i].tag, "free");

    occupys[i].start = -1;
    occupys[i].length = 0;
    strcpy(occupys[i].tag, "");
  }
  free_quantity = 0;
  occupy_quantity = 0;
}

//读数据函数
int readData()
{
  FILE *fp;
  // char fname[20];

  cout << "请输入初始空闲表文件名:";
  // cin >> fname;
  char fname[] = "mem_alloc_table.txt";

  if ((fp = fopen(fname, "r")) == NULL)
  {
    cout << "错误,文件打不开,请检查文件名" << endl;
  }
  else
  {
    while (!feof(fp))
    {
      fscanf(fp, "%d %d", &frees[free_quantity].start, &frees[free_quantity].length);
      free_quantity++;
    }
    return 1;
  }
  return 0;
}

// sort
void sort()
{
  int i, j, p;

  for (i = 0; i < free_quantity - 1; i++)
  {
    p = i;
    for (j = i + 1; j < free_quantity; j++)
    {
      if (frees[j].start < frees[p].start)
      {
        p = j;
      }
    }
    if (p != i)
    {
      frees[free_quantity] = frees[i];
      frees[i] = frees[p];
      frees[p] = frees[free_quantity];
    }
  }
}

//显示函数
void view()
{
  int i;

  cout << endl
       << "----------------------------------------------------------" << endl;
  cout << "当前空闲表:" << endl;
  cout << "起始地址    长度    状态" << endl;
  for (i = 0; i < free_quantity; i++)
  {
    // cout.setf(2);
    // cout.width(12);
    cout << frees[i].start;
    cout.width(12);
    cout << frees[i].length;
    cout.width(8);
    cout << frees[i].tag << endl;
  }

  cout << endl
       << "----------------------------------------------------------" << endl;
  cout << "当前已分配表:" << endl;
  cout << "起始地址    长度   占用作业名" << endl;
  for (i = 0; i < occupy_quantity; i++)
  {
    cout.setf(2);
    cout.width(12);
    cout << occupys[i].start;
    cout.width(10);
    cout << occupys[i].length;
    cout.width(8);
    cout << occupys[i].tag << endl;
  }
}

//最先适应分配算法
void first_fit()
{
  char job_name[20];
  int job_length;
  int i, j, flag, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";

  cin >> job_name;
  cin >> job_length;

  flag = 0;
  for (i = 0; i < free_quantity; i++)
  {
    if (frees[i].length >= job_length)
    {
      flag = 1;
    }
  }
  if (flag == 0)
  {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  }
  else
  {
    t = 0;
    i = 0;
    while (t == 0)
    {
      if (frees[i].length >= job_length)
      {
        t = 1;
      }
      i++;
    }
    i--;
    occupys[occupy_quantity].start = frees[i].start;
    strcpy(occupys[occupy_quantity].tag, job_name);
    occupys[occupy_quantity].length = job_length;
    occupy_quantity++;

    if (frees[i].length > job_length)
    {
      frees[i].start += job_length;
      frees[i].length -= job_length;
    }
    else
    {
      for (j = i; j < free_quantity - 1; j++)
      {
        frees[j] = frees[j + 1];
      }
      free_quantity--;
      cout << "内存空间成功:)" << endl;
    }
  }
}

//最优适应分配算法
void best_fit()
{
  char job_name[20];
  int job_length;
  int i, j, flag, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";
  cin >> job_name;
  cin >> job_length;

  flag = 0;
  for (i = 0; i < free_quantity; i++)
  {
    if (frees[i].length >= job_length)
    {
      flag = 1;
    }
  }
  if (flag == 0)
  {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  }
  else
  {
    t = 0;
    i = 0;
    while (t == 0)
    {
      if (frees[i].length >= job_length)
      {
        t = 1;
      }
      i++;
    }
    i--;
    for (j = 0; j < free_quantity; j++)
    {
      if ((frees[j].length >= job_length) && (frees[j].length < frees[i].length))
      {
        i = j;
      }
    }

    occupys[occupy_quantity].start = frees[i].start;
    strcpy(occupys[occupy_quantity].tag, job_name);
    occupys[occupy_quantity].length = job_length;
    occupy_quantity++;

    if (frees[i].length > job_length)
    {
      frees[i].start += job_length;
      frees[i].length -= job_length;
    }
    else
    {
      for (j = i; j < free_quantity - 1; j++)
      {
        frees[j] = frees[j + 1];
      }
      free_quantity--;
      cout << "内存空间成功:)" << endl;
    }
  }
}

//最坏适应算法
void worst_fit()
{
  char job_name[20];
  int job_length;
  int i, j, flag, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";
  cin >> job_name;
  cin >> job_length;

  flag = 0;
  for (i = 0; i < free_quantity; i++)
  {
    if (frees[i].length >= job_length)
    {
      flag = 1;
    }
  }
  if (flag == 0)
  {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  }
  else
  {
    t = 0;
    i = 0;
    while (t == 0)
    {
      if (frees[i].length >= job_length)
      {
        t = 1;
      }
      i++;
    }
    i--;
    for (j = 0; j < free_quantity; j++)
    {
      if ((frees[j].length >= job_length) && (frees[j].length > frees[i].length))
      {
        i = j;
      }
    }

    occupys[occupy_quantity].start = frees[i].start;
    strcpy(occupys[occupy_quantity].tag, job_name);
    occupys[occupy_quantity].length = job_length;
    occupy_quantity++;

    if (frees[i].length > job_length)
    {
      frees[i].start += job_length;
      frees[i].length -= job_length;
    }
    else
    {
      for (j = i; j < free_quantity - 1; j++)
      {
        frees[j] = frees[j + 1];
      }
      free_quantity--;
      cout << "内存空间成功:)" << endl;
    }
  }
}

//撤消作业
void finished()
{
  char job_name[20];
  int i, j, flag, p = 0;
  int start;
  int length;

  cout << "请输入要撤消的作业名:";
  cin >> job_name;

  flag = -1;
  for (i = 0; i < occupy_quantity; i++)
  {
    if (!strcmp(occupys[i].tag, job_name))
    {
      flag = i;
      start = occupys[i].start;
      length = occupys[i].length;
    }
  }
  if (flag == -1)
  {
    cout << "没有这个作业名" << endl;
  }
  else
  {
    //加入空闲表
    for (i = 0; i < free_quantity; i++)
    {
      if ((frees[i].start + frees[i].length) == start)
      {
        if (((i + 1) < free_quantity) && (frees[i + 1].start == start + length))
        {
          frees[i].length = frees[i].length + frees[i + 1].length + length;
          for (j = i + 1; j < free_quantity; j++)
          {
            frees[j] = frees[j + 1];
          }
          free_quantity--;
          p = 1;
        }
        else
        {
          frees[i].length += length;
          p = 1;
        }
      }
      if (frees[i].start == (start + length))
      {
        frees[i].start = start;
        frees[i].length += length;
        p = 1;
      }
    }

    if (p == 0)
    {
      frees[free_quantity].start = start;
      frees[free_quantity].length = length;
      free_quantity++;
    }

    //删除分配表中的该作业

    for (i = flag; i < occupy_quantity; i++)
    {
      occupys[i] = occupys[i + 1];
    }
    occupy_quantity--;
  }
}

int main()
{

  int flag = 0;
  int t = 1;
  int chioce = 0;

  initial();

  flag = readData();

  while (flag == 1)
  {

    sort();
    cout << endl
         << endl
         << "=========================================================" << endl;
    cout << "              可变分区存储管理模拟系统" << endl;
    cout << "=========================================================" << endl;
    cout << "   1.申请空间   2.撤消作业   3.显示空闲表和分配表  0.退出" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce)
    {
    case 1:
      //换算法请换下一句调用
      first_fit();
      break;
    case 2:
      finished();
      break;
    case 3:
      view();
      break;
    case 0:
      flag = 0;
      break;
    default:
      cout << "选择错误!" << endl;
    }
  }
  return 0;
}
