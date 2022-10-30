/*
六、 作业调度
作业调度主要有FIFO，运算时间短的作业优先，优先数调度算法，响应比最高者优先调度算法，均衡调度算法


6月四号上午的考题
模拟最高响应比优先作业调度算法

输入：作业流文件，其中存储的是一系列要执行的作业，
每个作业包括三个数据项：
作业号、作业进入系统的时间（用一整数表示，如10：10，表示成1010）、估计执行时间（单位分）优先级（0级最高）
参数用空格隔开，下面是示例：
1 800 50
2 815 30
3 830 25
4 835 20
5 845 15
6 900 10
7 920 5
其中调度时刻为最后一个作业到达系统的时间！
输出作业号！进入内存的时间！每行输出一个作业信息


本程序包括:FIFO，运算时间短的作业优先，优先数调度算法，响应比最高者优先调度算法
*/

#include <iomanip>
#include <iostream>
using namespace std;

const int MAX_TASK_SIZE = 50; //定义最大作业

//定义数据结构体
typedef struct {
  int number;
  int arrival_time;
  int arrival_hour;
  int arrival_minute;
  int execution_time;

  int priority;

  float reponse_rate;

  int start_time;
  int turnaround_time;

  int finished;
} task;

task tasks[MAX_TASK_SIZE];
int task_size;

//初始化函数
void init() {
  int i;

  for (i = 0; i < MAX_TASK_SIZE; i++) {
    tasks[i].number = 0;
    tasks[i].arrival_time = 0;
    tasks[i].arrival_hour = 0;
    tasks[i].arrival_minute = 0;

    tasks[i].priority = 0;

    tasks[i].reponse_rate = 0;

    tasks[i].start_time = 0;
    tasks[i].turnaround_time = 0;

    tasks[i].finished = 0;
  }
  task_size = 0;
}

//重置作业数据函数
void reset() {
  for (int i = 0; i < MAX_TASK_SIZE; i++) {
    tasks[i].start_time = 0;
    tasks[i].turnaround_time = 0;

    tasks[i].finished = 0;
  }
}

//读入作业数据函数
void read_input() {
  FILE *fp;
  char fname[] = "inputs/tasks.txt";
  if ((fp = fopen(fname, "r")) == NULL) {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
  } else {
    while (!feof(fp)) {
      fscanf(fp, "%d %d %d %d", &tasks[task_size].number,
             &tasks[task_size].arrival_time, &tasks[task_size].execution_time,
             &tasks[task_size].priority);
      tasks[task_size].arrival_hour = tasks[task_size].arrival_time / 100;
      tasks[task_size].arrival_minute = tasks[task_size].arrival_time % 100;
      task_size++;
    }
    //输出初始作业数据
    cout << "输出初始作业数据" << endl;
    cout << "-----------------------" << endl;
    cout.setf(2);
    cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14)
         << "所需时间(分)" << setw(14) << "优先级(0>1)" << endl;

    for (int i = 0; i < task_size; i++) {
      cout << setw(10) << tasks[i].number << setw(12) << tasks[i].arrival_time
           << setw(14) << tasks[i].execution_time << setw(14)
           << tasks[i].priority << endl;
    }
  }
}

// FIFO算法
void fifo() {

  //输出作业流
  cout << endl;
  cout << endl << "FIFO算法作业流" << endl;
  cout << "---------------------------------------------------------------"
       << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(12)
       << "开始时间" << setw(14) << "周转时间(分)" << endl;

  int current_hour = tasks[0].arrival_hour;
  int current_minute = tasks[0].arrival_minute;
  int turnaround_time = 0;
  for (int i = 0; i < task_size; i++) {
    tasks[i].start_time = current_hour * 100 + current_minute;
    tasks[i].turnaround_time = (current_hour - tasks[i].arrival_hour) * 60 +
                               (current_minute - tasks[i].arrival_minute) +
                               tasks[i].execution_time;

    cout << setw(10) << tasks[i].number << setw(12) << tasks[i].arrival_time
         << setw(12) << tasks[i].start_time << setw(14)
         << tasks[i].turnaround_time << endl;

    current_hour =
        current_hour + (tasks[i].execution_time + current_minute) / 60;
    current_minute = (tasks[i].execution_time + current_minute) % 60;

    turnaround_time += tasks[i].turnaround_time;
  }

  cout << endl
       << "总周转时间:" << turnaround_time
       << "   平均周转时间:" << turnaround_time / task_size << endl;
}

//运算时间短的作业优先算法
void srtn() {
  //输出作业流
  cout << endl;
  cout << endl
       << "时间短作业优先算法作业流(开始调度时刻为最后一个作业到达系统的时间)"
       << endl;
  cout << "--------------------------------------------------------------------"
          "----"
       << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14)
       << "所需时间(分)" << setw(12) << "开始时间" << setw(14) << "周转时间(分)"
       << endl;

  int current_task_hour = tasks[task_size - 1].arrival_hour;
  int current_task_minute = tasks[task_size - 1].arrival_minute;
  int current_smallest_execution_time_task_position;
  int turnaround_time = 0;
  for (int i = 0; i < task_size; i++) {
    int current_task_execution_time = INT_MAX;
    for (int j = 0; j < task_size; j++) {
      if ((tasks[j].finished == 0) &&
          (tasks[j].execution_time < current_task_execution_time)) {
        current_smallest_execution_time_task_position = j;
        current_task_execution_time = tasks[j].execution_time;
      }
    }

    tasks[current_smallest_execution_time_task_position].start_time =
        current_task_hour * 100 + current_task_minute;
    tasks[current_smallest_execution_time_task_position].turnaround_time =
        (current_task_hour -
         tasks[current_smallest_execution_time_task_position].arrival_hour) *
            60 +
        (current_task_minute -
         tasks[current_smallest_execution_time_task_position].arrival_minute) +
        tasks[current_smallest_execution_time_task_position].execution_time;

    cout << setw(10)
         << tasks[current_smallest_execution_time_task_position].number
         << setw(12)
         << tasks[current_smallest_execution_time_task_position].arrival_time
         << setw(14)
         << tasks[current_smallest_execution_time_task_position].execution_time
         << setw(12)
         << tasks[current_smallest_execution_time_task_position].start_time
         << setw(14)
         << tasks[current_smallest_execution_time_task_position].turnaround_time
         << endl;

    current_task_hour =
        current_task_hour +
        (tasks[current_smallest_execution_time_task_position].execution_time +
         current_task_minute) /
            60;
    current_task_minute =
        (tasks[current_smallest_execution_time_task_position].execution_time +
         current_task_minute) %
        60;

    tasks[current_smallest_execution_time_task_position].finished = 1;

    turnaround_time +=
        tasks[current_smallest_execution_time_task_position].turnaround_time;
  }
  cout << endl
       << "总周转时间:" << turnaround_time
       << "   平均周转时间:" << turnaround_time / task_size << endl;
}

//优先数调度算法
void hpf() {
  //输出作业流
  cout << endl;
  cout << endl
       << "优先数调度算法作业流(开始调度时刻为最后一个作业到达系统的时间)"
       << endl;
  cout << "--------------------------------------------------------------------"
          "----"
       << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14)
       << "优先级(0>1)" << setw(12) << "开始时间" << setw(14) << "周转时间(分)"
       << endl;

  int current_task_hour = tasks[task_size - 1].arrival_hour;
  int current_task_minute = tasks[task_size - 1].arrival_minute;
  int current_highest_priority_task_position = INT_MAX;
  int turnaround_time = 0;
  for (int i = 0; i < task_size; i++) {
    int current_task_priority = INT_MAX;
    for (int j = 0; j < task_size; j++) {
      if ((tasks[j].finished == 0) &&
          (tasks[j].priority < current_task_priority)) {
        current_highest_priority_task_position = j;
        current_task_priority = tasks[j].priority;
      }
    }
    tasks[current_highest_priority_task_position].start_time =
        current_task_hour * 100 + current_task_minute;
    tasks[current_highest_priority_task_position].turnaround_time =
        (current_task_hour -
         tasks[current_highest_priority_task_position].arrival_hour) *
            60 +
        (current_task_minute -
         tasks[current_highest_priority_task_position].arrival_minute) +
        tasks[current_highest_priority_task_position].execution_time;

    cout << setw(10) << tasks[current_highest_priority_task_position].number
         << setw(12)
         << tasks[current_highest_priority_task_position].arrival_time
         << setw(14) << tasks[current_highest_priority_task_position].priority
         << setw(12) << tasks[current_highest_priority_task_position].start_time
         << setw(14)
         << tasks[current_highest_priority_task_position].turnaround_time
         << endl;

    current_task_hour =
        current_task_hour +
        (tasks[current_highest_priority_task_position].execution_time +
         current_task_minute) /
            60;
    current_task_minute =
        (tasks[current_highest_priority_task_position].execution_time +
         current_task_minute) %
        60;

    tasks[current_highest_priority_task_position].finished = 1;

    turnaround_time +=
        tasks[current_highest_priority_task_position].turnaround_time;
  }
  cout << endl
       << "总周转时间:" << turnaround_time
       << "   平均周转时间:" << turnaround_time / task_size << endl;
}

//响应比最高者优先调度算法
void hrrf() {

  //输出作业流
  cout << endl;
  cout << endl
       << "响应比高者优先调度算法作业流("
          "开始调度时刻为最后一个作业到达系统的时间)"
       << endl;
  cout << "--------------------------------------------------------------------"
       << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(12)
       << "开始时间" << setw(14) << "周转时间(分)" << endl;

  int current_task_hour = tasks[task_size - 1].arrival_hour;
  int current_task_minute = tasks[task_size - 1].arrival_minute;
  int current_highest_response_rate_task_position = -1;
  int turnaround_time = 0;
  for (int i = 0; i < task_size; i++) {
    float current_response_rate = -1;
    // Calculate and set the respose rate
    for (int j = 0; j < task_size; j++) {
      if (tasks[j].finished == 0) {
        tasks[j].turnaround_time =
            (current_task_hour - tasks[j].arrival_hour) * 60 +
            (current_task_minute - tasks[j].arrival_minute);
        tasks[j].reponse_rate =
            (float)(tasks[j].turnaround_time / tasks[j].execution_time);
      }
    }

    // Choose the task with the higest respones rate
    for (int j = 0; j < task_size; j++) {
      if ((tasks[j].finished == 0) &&
          (tasks[j].reponse_rate > current_response_rate)) {
        current_highest_response_rate_task_position = j;
        current_response_rate = tasks[j].reponse_rate;
      }
    }
    // Set the start time of the choosen task and recalculate response rate
    tasks[current_highest_response_rate_task_position].start_time =
        current_task_hour * 100 + current_task_minute;
    tasks[current_highest_response_rate_task_position].turnaround_time =
        (current_task_hour -
         tasks[current_highest_response_rate_task_position].arrival_hour) *
            60 +
        (current_task_minute -
         tasks[current_highest_response_rate_task_position].arrival_minute) +
        tasks[current_highest_response_rate_task_position].execution_time;

    cout << setw(10)
         << tasks[current_highest_response_rate_task_position].number
         << setw(12)
         << tasks[current_highest_response_rate_task_position].arrival_time
         << setw(12)
         << tasks[current_highest_response_rate_task_position].start_time
         << setw(14)
         << tasks[current_highest_response_rate_task_position].turnaround_time
         << endl;

    current_task_hour =
        current_task_hour +
        (tasks[current_highest_response_rate_task_position].execution_time +
         current_task_minute) /
            60;
    current_task_minute =
        (tasks[current_highest_response_rate_task_position].execution_time +
         current_task_minute) %
        60;

    tasks[current_highest_response_rate_task_position].finished = 1;

    turnaround_time +=
        tasks[current_highest_response_rate_task_position].turnaround_time;
  }
  cout << endl
       << "总周转时间:" << turnaround_time
       << "   平均周转时间:" << turnaround_time / task_size << endl;
}

int main() {
  init();
  read_input();
  fifo();
  srtn();
  reset();
  hpf();
  reset();
  hrrf();
  return 0;
}
