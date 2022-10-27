/*
五 磁盘存储空间的管理
主要有：
位示图 和内存的位示差不多
空闲块表 和可变内存管理差不多
空闲块链 主要是UNIX成组链接法的设计与实现

UNIX系统文件管理组连接算法：

把空闲块分成若干组，把指向一组中各空闲块的指针集中一起。
这样既可方便查找，又可减少为修改指针而启动磁盘的次数。
UNIX系统：采用空闲块成组连接的方法。
UNIX系统把每100个空闲块作为一组，每一组的第一个空闲块中登记下一组空闲块的块号和空闲块数，
余下不足100块的那部分空闲块的块号及块数登记在一个专用块中，登记最后一组块号的那个空闲块其中第2个单元填“0”，
表示该块中指出的块号是最后一组的块号，空闲块链到此结束。
系统初始化时先把专用块内容读到内存，当需分配空闲块时，就直接在内存中可找到哪些块强障械模糠峙湟豢楹蟀芽障锌槭。
但要把一组中的第一个空闲块分配出去之前应把登记在该块中的下一组的块号及块数保存到专用块中。
当一组空闲块被分配完后，则再把专用块的内容读到内存，指出另一组可供分配的空闲块。当归还一块时，
只要把归还块的块号登记到当前组中且空闲块数加1。如果当前组已满100块，则把内存中的内容写到归还的那块中，
该归还块作为新组的第一块。假设初始化时系统已把专用块读入内存L单元开始的区域中，分配和回收的算法如下：
  分配一个空闲块
查L单元内容（空闲块数）：
当空闲块数1 i  ＝L＋空闲块数；
从i单元得到一空闲块号；
把该块分配给申请者；
空闲块数减1。
当空闲块数＝1 取出L＋1单元内容（一组的第一块块号或0）；
其值＝0无空闲块，申请者等待
不等于零把该块内容复制到专用块；
该块分配给申请者；
把专用块内容读到主存L开始的区域。
归还一块
查L单元的空闲块数；
当空闲块数100 空闲块数加1；
j  ＝L＋空闲块数；
归还块号填入j单元。
当空闲块数＝100 把主存中登记的信息写入归还块中；
把归还块号填入L＋1单元；
将L单元置成1。
采用成组连接后，分配回收磁盘块时均在内存中查找和修改，只是在一组空闲块分配完或空闲的磁盘块构成一组时才启动磁盘读写。
比单块连接方式效率高。

6月5日下午题是模拟UNIX的成组链接法的设计与实现

主要考的是利用文件输入一堆空闲块的号码，然后利用UNIX的成组链接法的管理方法，按照10块成一组，
并且可以实现输入一个数字N,然后把N个空闲块占用，输出专用块的大小，空闲号。如果专用块的空间不够，
把下一个成组的内容考入专用块。输出不要求写文件，但是要显示在屏幕上。

   本程序包括:UNIX的成组链接法的设计与实现

  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
  */

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

const int MAXGROUP = 10; //定义组的大小
const int MAXJOB = 100;  //定义一个作业最大能申请的块数

//结构体定义
typedef struct node
{
  int quantity;
  int cell[MAXGROUP];
  struct node *next;
} group;

typedef struct node1
{
  char name[20];
  int quantity;
  int cell[MAXJOB];
  struct node1 *next;
} job;

group *head;
int total;

job *jhead;

//初始化组函数
group *initial()
{
  int i;
  group *p;

  p = new group;

  p->quantity = 0;
  p->next = NULL;

  for (i = 0; i < MAXGROUP; i++)
  {
    p->cell[i] = -1;
  }

  return p;
}

//初始化作业函数
job *initial_job()
{
  int i;
  job *p;

  p = new job;

  strcpy(p->name, "");
  p->quantity = 0;
  p->next = NULL;

  for (i = 0; i < MAXGROUP; i++)
  {
    p->cell[i] = -1;
  }

  return p;
}

//读入空闲块流文件
void readData()
{
  FILE *fp;
  char fname[] = "free_disk_blocks.txt";
  int temp;
  group *p;

  cout << "请输入初始空闲块数据文件名:";

  if ((fp = fopen(fname, "r")) == NULL)
  {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
  }
  else
  {
    cout << "==================================================" << endl;
    cout << "读入的初始空闲块号:";
    while (!feof(fp))
    {
      fscanf(fp, "%d ", &temp);
      if (head->quantity < MAXGROUP)
      {
        head->cell[head->quantity] = temp;
        head->quantity++;
      }
      else
      {
        p = initial();
        p->next = head;
        head = p;
        p->cell[p->quantity] = temp;
        p->quantity++;
      }
      total++;
      //输出初始数据
      cout << temp << "  ";
    }

    cout << endl
         << "总空闲块数:" << total << endl;
  }
}

//查看专用块函数
void view()
{
  int i;

  cout << endl
       << "专用块数据如下:" << endl;
  cout << "-------------------------------" << endl;
  cout << "所存储的空闲块号:";
  for (i = 0; i < head->quantity; i++)
  {
    cout << head->cell[i] << "  ";
  }
  cout << endl
       << "专用块空闲块数为:" << head->quantity;
  cout << endl
       << "总空闲块数:" << total << endl;
}

//新申请函数
void bid()
{
  char jobname[20];
  int number;
  int i;
  job *p;

  cout << "----------------------------------" << endl;
  cout << "请输入新专业名:";
  cin >> jobname;
  cout << "所需内存块数:";
  cin >> number;

  if (number > total)
  {
    cout << "所需内存块数大于当前空闲块数,请稍候再试:)" << endl;
  }
  else
  {
    p = initial_job();
    strcpy(p->name, jobname);
    p->next = jhead->next;
    jhead->next = p;
    p->quantity = number;
    cout << "所申请到的空闲块号流:";
    for (i = 0; i < number; i++)
    {
      if (head->quantity > 1)
      {
        cout << head->cell[head->quantity - 1] << "  ";
        head->quantity--;
        p->cell[i] = head->cell[head->quantity - 1];
      }
      else
      {
        cout << head->cell[0] << "  ";
        p->cell[i] = head->cell[head->quantity - 1];
        head->quantity--;
        if (head->next != NULL)
        {
          head = head->next;
        }
      }
      total--;
    }
  }
  cout << endl;
}

//撤消作业
void finish()
{
  char jobname[20];
  int i;
  job *p, *q;
  group *r;

  cout << "请输入要撤消的作业名:";
  cin >> jobname;

  q = jhead;
  p = jhead->next;
  while ((p != NULL) && (strcmp(p->name, jobname)))
  {
    q = q->next;
    p = p->next;
  }

  if (p == NULL)
  {
    cout << "Sorry,没有该作业" << endl;
  }
  else
  {
    for (i = 0; i < p->quantity; i++)
    {
      if (head->quantity < MAXGROUP)
      {
        head->cell[head->quantity] = p->cell[i];
        head->quantity++;
      }
      else
      {
        r = initial();
        r->next = head;
        head = r;
        r->cell[r->quantity] = p->cell[i];
        r->quantity++;
      }
    }
    total += p->quantity;

    q->next = p->next;
    delete p;
  }
}

int main()
{
  int f = 1;
  int chioce;

  head = initial();
  total = 0;
  jhead = initial_job();

  readData();

  while (f == 1)
  {
    cout << "====================================================" << endl;
    cout << "           模拟UNIX的成组链接法    " << endl;
    cout << "====================================================" << endl;
    cout << "1.申请空闲块  2.撤消作业 3.查看专用块  0.退出" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce)
    {
    case 1:
      bid();
      break;
    case 2:
      finish();
      break;
    case 3:
      view();
      break;
    case 0:
      f = 0;
      break;
    default:
      cout << "选择错误!" << endl;
    }
  }
  return 0;
}
