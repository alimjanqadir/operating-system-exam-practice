/*
模拟死锁检测算法
1.输入：
“资源分配表”文件，每一行包含资源编号、进程编号两项（均用整数表示，并用空格分隔开），记录资源分配给了哪个进程。
“进程等待表”文件，每一行包含进程编号、资源编号两项（均用整数表示，并用空格分隔开），记录进程正在等待哪个资源。
下面是一个示例：
资源分配表：
1 1
2 2
3 3
进程等待表：
1 2
2 3
3 1
2.处理要求：
程序运行时，首先提示“请输入资源分配表文件的文件名：”；再提示“请输入进程等待表文件的文件名：”。
输入两个文件名后，程序将读入两个文件中的有关数据，并按照死锁检测算法进行检测。
3.输出要求：
第一行输出检测结果：有死锁 或 无死锁。
第二行输出进程循环等待队列，即进程编号（如果有死锁）。
4.文件名约定
提交的源程序名字：resourceXXX.c或者resourceXXX.cpp（依据所用语言确定）
输入文件名字：可由用户指定
结果输出到resultXXX.txt中
其中：XXX为账号。
5.死锁检测算法：当任一进程Pj申请一个已被其他进程占用的资源ri时，进行死锁检测。检测算法通过反复查找进程等待表和资源
分配表，来确定进程Pj对资源ri的请求是否导致形成环路，若是，便确定出现死锁。
6.测试说明：测试教师将事先准备好一组文件（格式为*.txt），从中为每个程序随机指定一至三个作为输入文件
（被测试者需从键盘输入指定文件的文件名），并查看程序输出结果。

本程序包括:死锁检测算法
*/

#include <iostream>
#include <string>

using namespace std;

const int MAX_PROCESS_SIZE = 100; //定义表的最大行数

typedef struct node {
  int resource;
  int process;
} proc;

proc allocated_resources[MAX_PROCESS_SIZE];
int allocated_resource_size;
proc waiting_processes[MAX_PROCESS_SIZE];
int waiting_process_size;

//初始化函数
void init() {
  int i;
  for (i = 0; i < MAX_PROCESS_SIZE; i++) {
    allocated_resources[i].process = -1;
    allocated_resources[i].resource = -1;

    waiting_processes[i].process = -1;
    waiting_processes[i].resource = -1;
  }
  allocated_resource_size = 0;
  waiting_process_size = 0;
}

//读数据文件
int readInput() {
  FILE *fp;
  char fname[40] = "resource_allocation_queue.txt";
  int i;

  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
    return 0;
  } else {
    while (!feof(fp)) {
      fscanf(fp, "%d %d",
             &allocated_resources[allocated_resource_size].resource,
             &allocated_resources[allocated_resource_size].process);
      allocated_resource_size++;
    }
  }

  strcpy(fname, "waiting_process_queue.txt");
  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
    return 0;
  } else {
    while (!feof(fp)) {
      fscanf(fp, "%d %d", &waiting_processes[waiting_process_size].process,
             &waiting_processes[waiting_process_size].resource);
      waiting_process_size++;
    }
  }

  //输出所读入的数据
  cout << endl << endl << "输出所读入的数据" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "资源分配表" << endl;
  cout << "资源编号  进程编号" << endl;

  for (i = 0; i < allocated_resource_size; i++) {
    cout << "  " << allocated_resources[i].resource << "     "
         << allocated_resources[i].process << endl;
  }
  cout << "───────────────────────" << endl;
  cout << "进程等待表" << endl;
  cout << "进程编号  资源编号" << endl;

  for (i = 0; i < waiting_process_size; i++) {
    cout << "  " << waiting_processes[i].resource << "     "
         << waiting_processes[i].process << endl;
  }
  return 1;
}

//检测
void check() {
  int table[MAX_PROCESS_SIZE][MAX_PROCESS_SIZE];
  int table1[MAX_PROCESS_SIZE][MAX_PROCESS_SIZE];

  int i, j, k;
  int flag, t, p;
  int max_process;

  //初始化表格
  for (i = 0; i < MAX_PROCESS_SIZE; i++) {
    for (j = 0; j < MAX_PROCESS_SIZE; j++) {
      table[i][j] = 0;
      table1[i][j] = 0;
    }
  }

  //先找到进程最大编号
  max_process = -1;
  for (i = 0; i < allocated_resource_size; i++) {
    if (allocated_resources[i].process > max_process) {
      max_process = allocated_resources[i].process;
    }
  }
  for (i = 0; i < waiting_process_size; i++) {
    if (waiting_processes[i].process > max_process) {
      max_process = waiting_processes[i].process;
    }
  }

  for (i = 0; i < waiting_process_size; i++) {
    for (j = 0; j < allocated_resource_size; j++) {
        if (waiting_processes[i].resource == allocated_resources[j].resource) {
        table[waiting_processes[i].process][allocated_resources[j].process] = 1;
        table1[waiting_processes[i].process][allocated_resources[j].process] =
            1;
      }
    }
  }

  cout << "初始等待占用表:" << endl;
  for (i = 0; i < max_process + 1; i++) {
    for (j = 0; j < max_process + 1; j++) {
      cout << table[i][j] << "  ";
    }
    cout << endl;
  }

  cout << endl;

  for (i = 0; i < max_process + 1; i++) {
    for (j = 0; j < max_process + 1; j++) {
      for (k = 0; k < max_process + 1; k++) {
        table[i][j] = table[i][j] || (table[i][k] && table[k][j]);
      }
    }
  }
  cout << "检测后的等待占用表:" << endl;

  for (i = 0; i < max_process + 1; i++) {
    for (j = 0; j < max_process + 1; j++) {
      cout << table[i][j] << "  ";
    }
    cout << endl;
  }

  flag = -1;
  for (i = 0; i < max_process + 1; i++) {
    if (table[i][i] == 1) {
      flag = i;
      break;
    }
  }
  cout << endl << endl << "检测结果" << endl;
  cout << "───────────────────" << endl;
  if (flag != -1) {
    cout << "存在死锁" << endl;
    cout << "进程循环等待队列:";

    p = flag; //存在进程循环等待队列的那一进程
    //进程循环等待队列中的所有进程是table表中的这一行是1的进程,只是顺序要再确定
    t = 1;
    while (t) {
      cout << p << "  ";
      for (j = 0; j < max_process + 1; j++) {
        if (table1[p][j] == 1) {
          if (table[j][flag] == 1) {
            p = j;
            break;
          }
        }
      }
      if (p == flag)
        t = 0;
    }
    cout << flag << endl;
  } else {
    cout << "不存在死锁" << endl;
  }
}

int main() {
  int flag;

  init();

  flag = readInput();

  if (flag)
    check();

  return 0;
}
