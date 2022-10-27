/*
七 磁盘调度

2001年的试题
磁盘调度算法
要求：
1。实现三种算法：
1。先来先服务
2。最短寻道优先（老师会给当前磁头的位置）
3。电梯算法
2。磁道服务顺序从指定的文本文件（TXT文件）中取出
3。输出：
第一行：磁道的服务顺序
第二行：显示移动总道数


   本程序包括:FIFO，最短寻道优先调度算法，电梯算法

  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include <iostream>

using namespace std;

const int MAXQUEUE = 200; //定义队列最大数

//结构体定义
typedef struct node
{
  int go;
  int visited;
} qu;

qu queue[MAXQUEUE];
int quantity;

int start; //定义开始时磁头所在位置

//初始化函数
void initial()
{
  int i;

  for (i = 0; i < MAXQUEUE; i++)
  {
    queue[i].go = -1;
    queue[i].visited = 0;
  }

  start = 53; //磁头的初始位置
}

//读入磁道号流
void readData()
{
  FILE *fp;
  char fname[] = "disk_tracks.txt";
  int temp, i;

  cout << "请输入磁道号流文件名:";

  if ((fp = fopen(fname, "r")) == NULL)
  {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
  }
  else
  {
    while (!feof(fp))
    {
      fscanf(fp, "%d ", &temp);
      queue[quantity].go = temp;
      quantity++;
    }
    cout << endl
         << "---------------------------------------------" << endl;
    cout << "所读入的磁道号流:";
    for (i = 0; i < quantity; i++)
    {
      cout << queue[i].go << "  ";
    }
    cout << endl
         << "请求数为:" << quantity << endl;
  }
}

// FIFO算法
void FIFO()
{
  int i;
  int total = 0;
  int current;

  cout << endl
       << "---------------------------------------------" << endl;
  cout << "FIFO算法的访问磁道号顺序流:";

  current = start;
  for (i = 0; i < quantity; i++)
  {
    cout << queue[i].go << "  ";
    total += abs(queue[i].go - current);
    current = queue[i].go;
  }
  cout << endl
       << "磁头移过的柱面数:" << total;
}

//最短寻道优先调度算法
void shortest()
{
  int i, j, p;
  int total = 0;
  int current;

  cout << endl
       << "---------------------------------------------" << endl;
  cout << "最短寻道优先调度算法的访问磁道号顺序流:";

  current = start;
  for (i = 0; i < quantity; i++)
  {
    p = 0;
    while (queue[p].visited != 0)
    {
      p++;
    }
    for (j = p; j < quantity; j++)
    {
      if ((queue[j].visited == 0) && (abs(current - queue[p].go) > abs(current - queue[j].go)))
      {
        p = j;
      }
    }
    cout << queue[p].go << "  ";
    total += abs(queue[p].go - current);
    queue[p].visited = 1;
    current = queue[p].go;
  }
  cout << endl
       << "磁头移过的柱面数:" << total;
}

//电梯算法
void elevator()
{
  int i, j, p, flag;
  int total = 0;
  int current;

  cout << endl
       << "---------------------------------------------" << endl;
  cout << "电梯调度算法" << endl;

  //磁头初始向里
  cout << "磁头初始向里的访问磁道号顺序流:";

  current = start;
  for (i = 0; i < quantity; i++)
  {
    flag = 1000;
    p = -1;
    for (j = 0; j < quantity; j++)
    {
      if ((queue[j].visited == 0) && (queue[j].go >= current))
      {
        if (abs(queue[j].go - current) < flag)
        {
          p = j;
          flag = abs(queue[j].go - current);
        }
      }
    }
    if (p != -1)
    {
      cout << queue[p].go << "  ";
      total += abs(queue[p].go - current);
      current = queue[p].go;
      queue[p].visited = 1;
    }
    else
    {
      for (j = 0; j < quantity; j++)
      {
        if ((queue[j].visited == 0) && (queue[j].go < current))
        {
          if (abs(queue[j].go - current) < flag)
          {
            p = j;
            flag = abs(queue[j].go - current);
          }
        }
      }
      cout << queue[p].go << "  ";
      total += abs(queue[p].go - current);
      current = queue[p].go;
      queue[p].visited = 1;
    }
  }
  cout << endl
       << "磁头移过的柱面数:" << total << endl;

  //磁头初始向外
  for (i = 0; i < quantity; i++)
  {
    queue[i].visited = 0;
  }
  total = 0;

  cout << "磁头初始向外的访问磁道号顺序流:";

  current = start;
  for (i = 0; i < quantity; i++)
  {
    flag = 1000;
    p = -1;
    for (j = 0; j < quantity; j++)
    {
      if ((queue[j].visited == 0) && (queue[j].go <= current))
      {
        if (abs(queue[j].go - current) < flag)
        {
          p = j;
          flag = abs(queue[j].go - current);
        }
      }
    }
    if (p != -1)
    {
      cout << queue[p].go << "  ";
      total += abs(queue[p].go - current);
      current = queue[p].go;
      queue[p].visited = 1;
    }
    else
    {
      for (j = 0; j < quantity; j++)
      {
        if ((queue[j].visited == 0) && (queue[j].go > current))
        {
          if (abs(queue[j].go - current) < flag)
          {
            p = j;
            flag = abs(queue[j].go - current);
          }
        }
      }
      cout << queue[p].go << "  ";
      total += abs(queue[p].go - current);
      current = queue[p].go;
      queue[p].visited = 1;
    }
  }
  cout << endl
       << "磁头移过的柱面数:" << total;
}

void main()
{
  int i;

  initial();

  readData();

  FIFO();

  shortest();

  for (i = 0; i < quantity; i++)
  {
    queue[i].visited = 0;
  }

  elevator();
}
