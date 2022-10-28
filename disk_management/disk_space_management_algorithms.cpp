/*
磁盘存储空间的管理
主要有：
位示图和内存的位示图管理相似
空闲块表和可变内存管理相似
空闲块链 主要是UNIX成组链接法的设计与实现

本程序包括:UNIX的成组链接法的设计与实现

UNIX系统文件管理组连接算法：
把空闲块分成若干组，把指向一组中各空闲块的指针集中一起。
这样既可方便查找，又可减少为修改指针而启动磁盘的次数。
UNIX系统：采用空闲块成组连接的方法。
UNIX系统把每100个空闲块作为一组，每一组的第一个空闲块中登记下一组空闲块的块号和空闲块数，
余下不足100块的那部分空闲块的块号及块数登记在一个专用块中，登记最后一组块号的那个空闲块其中第2个单元填“0”，
表示该块中指出的块号是最后一组的块号，空闲块链到此结束。

系统初始化时先把专用块内容读到内存，当需分配空闲块时，就直接在内存中可找到哪些块是空闲的，每分配一块后把空闲块数减一。
但要把一组中的第一个空闲块分配出去之前应把登记在该块中的下一组的块号及块数保存到专用块中。
当一组空闲块被分配完后，则再把专用块的内容读到内存，指出另一组可供分配的空闲块。当归还一块时，
只要把归还块的块号登记到当前组中，且空闲块数加1。如果当前组已满100块，则把内存中的内容写到归还的那块中，
该归还块作为新组的第一块。假设初始化时系统已把专用块读入内存L单元开始的区域中，分配和回收的算法如下：

（1）分配一个空闲块
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

（2）归还一块
查L单元的空闲块数；
当空闲块数100 空闲块数加1；
j  ＝L＋空闲块数；
归还块号填入j单元。
当空闲块数＝100 把主存中登记的信息写入归还块中；
把归还块号填入L＋1单元；
将L单元置成1。
采用成组连接后，分配回收磁盘块时均在内存中查找和修改，只是在一组空闲块分配完或空闲的磁盘块构成一组时才启动磁盘读写。
比单块连接方式效率高。

例题：
主要考的是利用文件输入一堆空闲块的号码，然后利用UNIX的成组链接法的管理方法，按照10块成一组，
并且可以实现输入一个数字N,然后把N个空闲块占用，输出专用块的大小，空闲号。如果专用块的空间不够，
把下一个成组的内容考入专用块。输出不要求写文件，但是要显示在屏幕上。

*/

#include <iostream>
using namespace std;

const int MAX_BLOCK_SIZE_IN_GROUP = 10;       //定义组的大小
const int MAX_TASK_SIZE = 100; //定义一个作业最大能申请的块数

//结构体定义
typedef struct node {
  int size;
  int blocks[MAX_BLOCK_SIZE_IN_GROUP];
  struct node *next;
} group;

typedef struct node1 {
  char name[20];
  int block_size;
  int blocks[MAX_TASK_SIZE];
  struct node1 *next;
} task;

group *groups_head;
int total_free_block_size;

task *tasks_head;

//初始化组函数
group *create_group() {
  int i;
  group *p;

  p = new group;

  p->size = 0;
  p->next = NULL;

  for (i = 0; i < MAX_BLOCK_SIZE_IN_GROUP; i++) {
    p->blocks[i] = -1;
  }

  return p;
}

//初始化作业函数
task *create_task() {
  int i;
  task *p;

  p = new task;

  strcpy(p->name, "");
  p->block_size = 0;
  p->next = NULL;

  for (i = 0; i < MAX_BLOCK_SIZE_IN_GROUP; i++) {
    p->blocks[i] = -1;
  }

  return p;
}

//读入空闲块流文件
void readInput() {
  FILE *fp;
  char fname[] = "free_disk_blocks.txt";
  int temp;
  group *p;

  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
  } else {
    cout << "==================================================" << endl;
    cout << "读入的初始空闲块号:";
    while (!feof(fp)) {
      fscanf(fp, "%d ", &temp);
      if (groups_head->size < MAX_BLOCK_SIZE_IN_GROUP) {
        groups_head->blocks[groups_head->size] = temp;
        groups_head->size++;
      } else {
        p = create_group();
        p->next = groups_head;
        groups_head = p;
        p->blocks[p->size] = temp;
        p->size++;
      }
      total_free_block_size++;
      //输出初始数据
      cout << temp << "  ";
    }

    cout << endl << "总空闲块数:" << total_free_block_size << endl;
  }
}

//查看专用块函数
void view() {
  int i;

  cout << endl << "专用块数据如下:" << endl;
  cout << "-------------------------------" << endl;
  cout << "所存储的空闲块号:";
  for (i = 0; i < groups_head->size; i++) {
    cout << groups_head->blocks[i] << "  ";
  }
  cout << endl << "专用块空闲块数为:" << groups_head->size;
  cout << endl << "总空闲块数:" << total_free_block_size << endl;
}

//新申请函数
void allocate() {
  char task_name[20];
  int block_size;
  int i;
  task *p;

  cout << "----------------------------------" << endl;
  cout << "请输入新专业名:";
  cin >> task_name;
  cout << "所需内存块数:";
  cin >> block_size;

  if (block_size > total_free_block_size) {
    cout << "所需内存块数大于当前空闲块数,请稍候再试:)" << endl;
  } else {
    p = create_task();
    strcpy(p->name, task_name);
    p->next = tasks_head->next;
    tasks_head->next = p;
    p->block_size = block_size;
    cout << "所申请到的空闲块号流:";
    for (i = 0; i < block_size; i++) {
      if (groups_head->size > 1) {
        cout << groups_head->blocks[groups_head->size - 1]
             << "  ";
        groups_head->size--;
        p->blocks[i] = groups_head->blocks[groups_head->size - 1];
      } else {
        cout << groups_head->blocks[0] << "  ";
        p->blocks[i] = groups_head->blocks[groups_head->size - 1];
        groups_head->size--;
        if (groups_head->next != NULL) {
          groups_head = groups_head->next;
        }
      }
      total_free_block_size--;
    }
  }
  cout << endl;
}

//撤消作业
void release() {
  char task_name[20];
  int i;
  task *p, *q;
  group *r;

  cout << "请输入要撤消的作业名:";
  cin >> task_name;

  q = tasks_head;
  p = tasks_head->next;
  while ((p != NULL) && (strcmp(p->name, task_name))) {
    q = q->next;
    p = p->next;
  }

  if (p == NULL) {
    cout << "Sorry,没有该作业" << endl;
  } else {
    for (i = 0; i < p->block_size; i++) {
      if (groups_head->size < MAX_BLOCK_SIZE_IN_GROUP) {
        groups_head->blocks[groups_head->size] = p->blocks[i];
        groups_head->size++;
      } else {
        r = create_group();
        r->next = groups_head;
        groups_head = r;
        r->blocks[r->size] = p->blocks[i];
        r->size++;
      }
    }
    total_free_block_size += p->block_size;

    q->next = p->next;
    delete p;
  }
}

int main() {
  int f = 1;
  int chioce;

  groups_head = create_group();
  total_free_block_size = 0;
  tasks_head = create_task();

  readInput();

  while (f == 1) {
    cout << "====================================================" << endl;
    cout << "           模拟UNIX的成组链接法    " << endl;
    cout << "====================================================" << endl;
    cout << "1.申请空闲块  2.撤消作业 3.查看专用块  0.退出" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce) {
    case 1:
      allocate();
      break;
    case 2:
      release();
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
