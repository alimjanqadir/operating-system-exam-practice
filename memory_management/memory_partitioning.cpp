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
*/

#include <iostream>
using namespace std;

const int MAX_TASK_SIZE = 100; //定义表最大记录数

typedef struct node {
  int start;
  int length;
  char tag[20];
} task;

task free_sections[MAX_TASK_SIZE]; //定义空闲区表
int free_section_size;

task allocated_sections[MAX_TASK_SIZE]; //定义已分配区表
int allocated_section_size;

//初始化函数
void init() {
  int i;

  for (i = 0; i < MAX_TASK_SIZE; i++) {
    free_sections[i].start = -1;
    free_sections[i].length = 0;
    strcpy(free_sections[i].tag, "free");

    allocated_sections[i].start = -1;
    allocated_sections[i].length = 0;
    strcpy(allocated_sections[i].tag, "");
  }
  free_section_size = 0;
  allocated_section_size = 0;
}

//读数据函数
int readInput() {
  FILE *fp;
  char fname[] = "mem_alloc_table.txt";

  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名" << endl;
  } else {
    while (!feof(fp)) {
      fscanf(fp, "%d %d", &free_sections[free_section_size].start,
             &free_sections[free_section_size].length);
      free_section_size++;
    }
    return 1;
  }
  return 0;
}

// sort
void sort() {
  int i, j, p;

  for (i = 0; i < free_section_size - 1; i++) {
    p = i;
    for (j = i + 1; j < free_section_size; j++) {
      if (free_sections[j].start < free_sections[p].start) {
        p = j;
      }
    }
    if (p != i) {
      free_sections[free_section_size] = free_sections[i];
      free_sections[i] = free_sections[p];
      free_sections[p] = free_sections[free_section_size];
    }
  }
}

//显示函数
void view() {
  int i;

  cout << endl
       << "----------------------------------------------------------" << endl;
  cout << "当前空闲表:" << endl;
  cout << "起始地址    长度    状态" << endl;
  for (i = 0; i < free_section_size; i++) {
    // cout.setf(2);
    // cout.width(12);
    cout << free_sections[i].start;
    cout.width(12);
    cout << free_sections[i].length;
    cout.width(8);
    cout << free_sections[i].tag << endl;
  }

  cout << endl
       << "----------------------------------------------------------" << endl;
  cout << "当前已分配表:" << endl;
  cout << "起始地址    长度   占用作业名" << endl;
  for (i = 0; i < allocated_section_size; i++) {
    // cout.setf(2);
    // cout.width(12);
    cout << allocated_sections[i].start;
    cout.width(12);
    cout << allocated_sections[i].length;
    cout.width(8);
    cout << allocated_sections[i].tag << endl;
  }
}

//最先适应分配算法
void first_fit() {
  char task_name[20];
  int task_length;
  int i, j, has_free_section, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";
  cin >> task_name;
  cin >> task_length;

  // Check has free section
  has_free_section = 0;
  for (i = 0; i < free_section_size; i++) {
    if (free_sections[i].length >= task_length) {
      has_free_section = 1;
    }
  }
  if (has_free_section == 0) {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  } else {
    t = 0;
    i = 0;

    // Find the free section
    while (t == 0) {
      if (free_sections[i].length >= task_length) {
        t = 1;
      }
      i++;
    }
    i--;
    // Update allocated sections
    allocated_sections[allocated_section_size].start = free_sections[i].start;
    strcpy(allocated_sections[allocated_section_size].tag, task_name);
    allocated_sections[allocated_section_size].length = task_length;
    allocated_section_size++;

    // Update free sections
    if (free_sections[i].length > task_length) {
      free_sections[i].start += task_length;
      free_sections[i].length -= task_length;
    } else {
      for (j = i; j < free_section_size - 1; j++) {
        free_sections[j] = free_sections[j + 1];
      }
      free_section_size--;
    }
    cout << "内存空间成功:)" << endl;
  }
}

//最优适应分配算法
void best_fit() {
  char task_name[20];
  int task_length;
  int i, j, has_free_section, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";
  cin >> task_name;
  cin >> task_length;

  has_free_section = 0;
  for (i = 0; i < free_section_size; i++) {
    if (free_sections[i].length >= task_length) {
      has_free_section = 1;
    }
  }
  if (has_free_section == 0) {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  } else {
    t = 0;
    i = 0;
    while (t == 0) {
      if (free_sections[i].length >= task_length) {
        t = 1;
      }
      i++;
    }
    i--;
    for (j = 0; j < free_section_size; j++) {
      if ((free_sections[j].length >= task_length) &&
          (free_sections[j].length < free_sections[i].length)) {
        i = j;
      }
    }

    allocated_sections[allocated_section_size].start = free_sections[i].start;
    strcpy(allocated_sections[allocated_section_size].tag, task_name);
    allocated_sections[allocated_section_size].length = task_length;
    allocated_section_size++;

    if (free_sections[i].length > task_length) {
      free_sections[i].start += task_length;
      free_sections[i].length -= task_length;
    } else {
      for (j = i; j < free_section_size - 1; j++) {
        free_sections[j] = free_sections[j + 1];
      }
      free_section_size--;
    }
    cout << "内存空间成功:)" << endl;
  }
}

//最坏适应算法
void worst_fit() {
  char task_name[20];
  int task_length;
  int i, j, has_free_section, t;

  cout << "请输入新申请内存空间的作业名和空间大小:";
  cin >> task_name;
  cin >> task_length;

  has_free_section = 0;
  for (i = 0; i < free_section_size; i++) {
    if (free_sections[i].length >= task_length) {
      has_free_section = 1;
    }
  }
  if (has_free_section == 0) {
    cout << endl
         << "Sorry,当前没有能满足你申请长度的空闲内存,请稍候再试" << endl;
  } else {
    t = 0;
    i = 0;
    while (t == 0) {
      if (free_sections[i].length >= task_length) {
        t = 1;
      }
      i++;
    }
    i--;
    for (j = 0; j < free_section_size; j++) {
      if ((free_sections[j].length >= task_length) &&
          (free_sections[j].length > free_sections[i].length)) {
        i = j;
      }
    }

    allocated_sections[allocated_section_size].start = free_sections[i].start;
    strcpy(allocated_sections[allocated_section_size].tag, task_name);
    allocated_sections[allocated_section_size].length = task_length;
    allocated_section_size++;

    if (free_sections[i].length > task_length) {
      free_sections[i].start += task_length;
      free_sections[i].length -= task_length;
    } else {
      for (j = i; j < free_section_size - 1; j++) {
        free_sections[j] = free_sections[j + 1];
      }
      free_section_size--;
    }
    cout << "内存空间成功:)" << endl;
  }
}

//撤消作业
void finish_task() {
  char task_name[20];
  int i, j, t, p = 0;
  int start;
  int length;

  cout << "请输入要撤消的作业名:";
  cin >> task_name;

  t = -1;
  for (i = 0; i < allocated_section_size; i++) {
    if (!strcmp(allocated_sections[i].tag, task_name)) {
      t = i;
      start = allocated_sections[i].start;
      length = allocated_sections[i].length;
    }
  }
  if (t == -1) {
    cout << "没有这个作业名" << endl;
  } else {
    //加入空闲表
    for (i = 0; i < free_section_size; i++) {
      // Find the neighboring section, it checks for section above the newly
      // freed section
      if ((free_sections[i].start + free_sections[i].length) == start) {
        // Is neighboring section is also free
        if (((i + 1) < free_section_size) &&
            (free_sections[i + 1].start == start + length)) {
          free_sections[i].length =
              free_sections[i].length + free_sections[i + 1].length + length;
          for (j = i + 1; j < free_section_size; j++) {
            free_sections[j] = free_sections[j + 1];
          }
          free_section_size--;
          p = 1;
        } else {
          free_sections[i].length += length;
          p = 1;
        }
      }
      // Find the neighboring section, this check for section below the newly
      // freed section
      if (free_sections[i].start == (start + length)) {
        free_sections[i].start = start;
        free_sections[i].length += length;
        p = 1;
      }
    }

    // No merging
    if (p == 0) {
      free_sections[free_section_size].start = start;
      free_sections[free_section_size].length = length;
      free_section_size++;
    }

    //删除分配表中的该作业
    for (i = t; i < allocated_section_size; i++) {
      allocated_sections[i] = allocated_sections[i + 1];
    }
    allocated_section_size--;
  }
}

int main() {
  int flag = 0;
  int t = 1;
  int chioce = 0;

  init();

  flag = readInput();

  while (flag == 1) {

    sort();
    cout << endl
         << endl
         << "=========================================================" << endl;
    cout << "              可变分区存储管理模拟系统" << endl;
    cout << "=========================================================" << endl;
    cout << "   1.申请空间   2.撤消作业   3.显示空闲表和分配表  0.退出" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce) {
    case 1:
      //换算法请换下一句调用
      first_fit();
      break;
    case 2:
      finish_task();
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
