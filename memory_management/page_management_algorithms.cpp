/*
页式存储管理方案

  位示图

内存被划分成2048块（页）。用32位字长的字存放位示图，为0的位表示该块尚未分配，为1的位表示该块已分配
实习检查：

1、运行程序，由检查教师给出文件名，该文件中存有内存目前状况的位示图的数据（0和1的文件）。（程序应做提示，界面友好）。
2、你所编制的程序应读入数据，存放在相应的数据结构中。
3、显示友好的用户界面，由检查教师输入内存申请（总块数）。
4、根据申请和位示图状态，为用户分配内存，并建立页表。
5、输出位示图和页表。


本程序包括主要实现位示图算法,比较简单
*/
#include <iostream>
using namespace std;

const int PAGES = 256; //定义总块数
const int WORD = 32;   //定义字长

const int WORDNUM = PAGES / WORD; //定义总字数

typedef struct node {
  char name[20];
  int page_size;
  int page_numbers[PAGES];
  struct node *next;
} task;

int table[WORDNUM][WORD];
int free_page_size = 0;
task *head;

//初始化函数
void init() {
  int i, j;
  task *p;

  //初始化位示图
  for (i = 0; i < WORDNUM; i++) {
    for (j = 0; j < WORD; j++) {
      table[i][j] = 0;
    }
  }

  //初始化作业表头
  p = new task;
  strcpy(p->name, "null");
  p->page_size = 0;
  p->next = NULL;
  head = p;
}

//读入位示图初始数据
void readInput() {
  int i, j;
  FILE *fp;
  char fname[] = "page_allocation_bitmap.txt";

  cout << "请输入初始位示图数据文件名:" << endl;

  if ((fp = fopen(fname, "r")) != NULL) {
    for (i = 0; i < WORDNUM; i++) {
      for (j = 0; j < WORD; j++) {
        fscanf(fp, "%d", &table[i][j]);
        if (table[i][j] == 0)
          free_page_size++;
      }
    }
    cout << endl;
    cout << "初始位示图" << endl;
    for (i = 0; i < WORDNUM; i++) {
      for (j = 0; j < WORD; j++) {
        cout << table[i][j] << "";
      }
      cout << endl;
    }
    cout << "总空闲块数:" << free_page_size;
  } else {
    cout << "文件不能打开" << endl;
  }
}

//新加入作业函数
void add() {
  char task_name[20];
  int page_size;
  task *p;
  int i, j, k = 0;

  cout << "请输入新增的作业名:";
  cin >> task_name;
  cout << "新增作业所需页数:";
  cin >> page_size;

  if (page_size <= free_page_size) {
    free_page_size -= page_size;

    p = new task;
    strcpy(p->name, task_name);
    p->page_size = page_size;

    for (k = 0; k < page_size; k++) {
      i = 0;
      j = 0;
      while (table[i][j] == 1) {
        j = 0;
        while (table[i][j] == 1)
          j++;
        if (table[i][j] == 1)
          i++;
      }
      p->page_numbers[k] = i * WORD + j;
      table[i][j] = 1;
    }

    p->next = head->next;
    head->next = p;
  } else {
    cout << "错误,当前剩余页数小于所需页数,请稍候再试:)" << endl;
  }
}

//完成作业函数
void release() {
  char task_name[20];

  task *p, *q;
  int n, i, j, page_number, k;

  cout << "请输入完成的作业名:";
  cin >> task_name;

  p = head->next;
  q = head;

  while (p != NULL) {
    if (strcmp(p->name, task_name) == 0) {
      break;
    }
    p = p->next;
    q = q->next;
  }

  page_number = p->page_size;
  for (k = 0; k < page_number; k++) {
    n = p->page_numbers[k];
    i = n / WORD;
    j = n % WORD;
    table[i][j] = 0;
  }

  free_page_size += page_number;
  q->next = p->next;
  delete p;
}

//显示当前位示图函数
void view_table() {
  int i, j;
  cout << "当前位示图" << endl;
  for (i = 0; i < WORDNUM; i++) {
    for (j = 0; j < WORD; j++) {
      cout << table[i][j] << " ";
    }
    cout << endl;
  }
  cout << "总空闲块数:" << free_page_size << endl;
}

//显示所有页表函数
void view_pages() {
  task *p;
  int i;

  p = head->next;
  if (p == NULL)
    cout << "当前没有用户作业" << endl;
  else
    cout << "当前所有的用户作业页表情况" << endl;
  while (p != NULL) {
    cout << "作业名:" << p->name << "    所用块数:" << p->page_size << endl;
    cout << "本作业所点块的序列是:" << endl;
    for (i = 0; i < p->page_size; i++) {
      cout << p->page_numbers[i] << "  ";
    }
    cout << endl;
    p = p->next;
  }
}

int main() {
  int t = 1, chioce;

  init();

  readInput();

  while (t == 1) {
    cout << endl << "===========================================" << endl;
    cout << "            页式内存管理系统模拟程序" << endl;
    cout << "===========================================" << endl;
    cout << "1.加入新作业    2.完成作业    3.显示当前内存位示图    "
            "4.显示所有作业页表  0.退出"
         << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce) {
    case 1:
      add();
      break;
    case 2:
      release();
      break;
    case 3:
      view_table();
      break;
    case 4:
      view_pages();
      break;
    case 0:
      t = 0;
      break;
    default:
      cout << "选择错误" << endl;
    }
  }
  return 0;
}
