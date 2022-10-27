/*
一、 进程调度
进程调度算法有FIFO，优先数调度算法，时间片轮转调度算法，分级调度算法，目前主要是考FIFO和优先数调度算法（静态优先级）。

输入：进程流文件，其中存储的是一系列要执行的进程，
每个作业包括四个数据项：

进程名 进程状态(1就绪 2等待 3运行)  所需时间  优先数(0级最高)

输出:
进程执行流 和等待时间 平均等待时间

本程序包括:FIFO，优先数调度算法，时间片轮转调度算法

  */
#include <iostream>
using namespace std;

const int TIME_INTERVAL = 10; //定义时间片的长度为10秒
const int MAX_PCB_SIZE = 100; //定义最大进程数

//定义进程结构体
typedef struct node {
  char name[20];
  int status;
  int time;
  int priority;

  int finished;
  int waiting_time;
} pcb;

pcb pcbs[MAX_PCB_SIZE];
int pcb_size;

//初始化函数
void init() {
  int i;

  for (i = 0; i < MAX_PCB_SIZE; i++) {
    strcpy(pcbs[i].name, "");
    pcbs[i].status = 0;
    pcbs[i].time = 0;
    pcbs[i].priority = 0;

    pcbs[i].waiting_time = 0;
    pcbs[i].finished = 0;
  }

  pcb_size = 0;
}

//读数据函数
int readInput() {
  FILE *fp;
  char fname[] = "inputs/processes.txt";
  int i;

  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名" << endl;
  } else {
    while (!feof(fp)) {
      fscanf(fp, "%s %d %d %d", pcbs[pcb_size].name, &pcbs[pcb_size].status,
             &pcbs[pcb_size].time, &pcbs[pcb_size].priority);
      pcb_size++;
    }

    //输出所读入的数据
    cout << "输出所读入的数据" << endl;
    cout << "进程名 进程状态  所需时间  优先数" << endl;
    for (i = 0; i < pcb_size; i++) {
      cout << "  " << pcbs[i].name << "     " << pcbs[i].status << "          "
           << pcbs[i].time << "        " << pcbs[i].priority << endl;
    }
    return (1);
  }
  return (0);
}

//重置数据,以供另一个算法使用
void reset() {
  int i;

  for (i = 0; i < MAX_PCB_SIZE; i++) {
    pcbs[i].finished = 0;
    pcbs[i].waiting_time = 0;
  }
}

//先进先出算法
void fifo() {
  int i, j;
  int total_waiting_time;

  //输出FIFO算法执行流
  cout << endl
       << "---------------------------------------------------------------"
       << endl;
  cout << "FIFO算法执行流:" << endl;
  cout << "进程名    等待时间" << endl;
  for (i = 0; i < pcb_size; i++) {
    cout << "  " << pcbs[i].name << "         " << pcbs[i].waiting_time << endl;

    for (j = i + 1; j < pcb_size; j++) {
      pcbs[j].waiting_time += pcbs[i].time;
    }
  }
  total_waiting_time = 0;
  for (i = 0; i < pcb_size; i++) {
    total_waiting_time += pcbs[i].waiting_time;
  }
  cout << "总等待时间:" << total_waiting_time
       << "  平均等待时间:" << total_waiting_time / pcb_size << endl;
}

//优先数调度算法
void hpf() {
  int i, j, p;
  int passed_time = 0;
  int total_waiting_time;

  int queue[MAX_PCB_SIZE];
  int current_priority = 1000;

  for (i = 0; i < pcb_size; i++) {
    current_priority = 1000;
    for (j = 0; j < pcb_size; j++) {
      if ((pcbs[j].finished == 0) && (pcbs[j].priority < current_priority)) {
        p = j;
        current_priority = pcbs[j].priority;
      }
    }
    queue[i] = p;
    pcbs[p].finished = 1;
    pcbs[p].waiting_time += passed_time;
    passed_time += pcbs[p].time;
  }

  //输出优先数调度执行流
  cout << endl
       << "---------------------------------------------------------------"
       << endl;
  cout << "优先数调度执行流:" << endl;
  cout << "进程名    等待时间" << endl;
  for (i = 0; i < pcb_size; i++) {
    cout << "  " << pcbs[queue[i]].name << "        "
         << pcbs[queue[i]].waiting_time << endl;
  }

  total_waiting_time = 0;
  for (i = 0; i < pcb_size; i++) {
    total_waiting_time += pcbs[i].waiting_time;
  }
  cout << "总等待时间:" << total_waiting_time
       << "  平均等待时间:" << total_waiting_time / pcb_size << endl;
}

//时间片轮转调度算法
void round_robin() {
  int i, j, number, flag = 1;
  int passed_time = 0;
  int max_time = 0;
  int round = 0;

  int queue[1000];
  int total = 0;

  while (flag == 1) {
    flag = 0;
    number = 0;

    // Find unfinished process count and last unfinished process pointer
    for (i = 0; i < pcb_size; i++) {
      if (pcbs[i].finished == 0) {
        number++;
        j = i;
      }
    }

    // Only one unfinished process left
    if (number == 1) {
      queue[total] = j;
      total++;
      pcbs[j].finished = 1;
    }

    if (number > 1) {
      for (i = 0; i < pcb_size; i++) {
        if (pcbs[i].finished == 0) {
          flag = 1;
          queue[total] = i;
          total++;
          if (pcbs[i].time <= TIME_INTERVAL * (round + 1)) {
            pcbs[i].finished = 1;
          }
        }
      }
    }

    round++;
  }

  if (queue[total - 1] == queue[total - 2]) {
    total--;
  }

  cout << endl
       << "---------------------------------------------------------------"
       << endl;
  cout << "时间片轮转调度执行流:";
  for (i = 0; i < total; i++) {
    cout << pcbs[queue[i]].name << "  ";
  }
}

//主函数
int main() {
  int flag;

  init();

  flag = readInput();

  if (flag == 1) {
    fifo();
    reset();

    hpf();
    reset();

    round_robin();
  }
  return 0;
}
