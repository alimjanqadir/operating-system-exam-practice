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


  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include <iostream>
#include <iomanip>
using namespace std;

const int MAXJOB = 50; //定义最大作业

//定义数据结构体
typedef struct node
{
  int number;
  int reach_time;
  int reach_hour;
  int reach_minite;
  int need_time;

  int privilege;

  float excellent;

  int start_time;
  int wait_time;

  int visited;
} job;

job jobs[MAXJOB];
int quantity;

//初始化函数
void initial()
{
  int i;

  for (i = 0; i < MAXJOB; i++)
  {
    jobs[i].number = 0;
    jobs[i].reach_time = 0;
    jobs[i].reach_hour = 0;
    jobs[i].reach_minite = 0;

    jobs[i].privilege = 0;

    jobs[i].excellent = 0;

    jobs[i].start_time = 0;
    jobs[i].wait_time = 0;

    jobs[i].visited = 0;
  }
  quantity = 0;
}

//重置作业数据函数
void reset()
{
  int i;

  for (i = 0; i < MAXJOB; i++)
  {
    jobs[i].start_time = 0;
    jobs[i].wait_time = 0;

    jobs[i].visited = 0;
  }
}

//读入作业数据函数
void readData()
{
  FILE *fp;
  char fname[] = "tasks.txt";
  int i;

  cout << "请输入作业数据文件名:";

  if ((fp = fopen(fname, "r")) == NULL)
  {
    cout << "错误,文件打不开,请检查文件名:)" << endl;
  }
  else
  {
    while (!feof(fp))
    {
      fscanf(fp, "%d %d %d %d", &jobs[quantity].number, &jobs[quantity].reach_time, &jobs[quantity].need_time, &jobs[quantity].privilege);
      jobs[quantity].reach_hour = jobs[quantity].reach_time / 100;
      jobs[quantity].reach_minite = jobs[quantity].reach_time % 100;
      quantity++;
    }
    //输出初始作业数据
    cout << "输出初始作业数据" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout.setf(2);
    cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14) << "所需时间(分)" << setw(14) << "优先级(0>1)" << endl;

    for (i = 0; i < quantity; i++)
    {
      cout << setw(10) << jobs[i].number << setw(12) << jobs[i].reach_time << setw(14) << jobs[i].need_time << setw(14) << jobs[i].privilege << endl;
    }
  }
}

// FIFO算法
void FIFO()
{
  int i;
  int current_hour;
  int current_minute;

  int total_time = 0;

  //输出作业流
  cout << endl;
  cout << endl
       << "FIFO算法作业流" << endl;
  cout << "---------------------------------------------------------------" << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(12) << "开始时间" << setw(14) << "周转时间(分)" << endl;

  current_hour = jobs[0].reach_hour;
  current_minute = jobs[0].reach_minite;
  for (i = 0; i < quantity; i++)
  {
    jobs[i].start_time = current_hour * 100 + current_minute;
    jobs[i].wait_time = (current_hour - jobs[i].reach_hour) * 60 + (current_minute - jobs[i].reach_minite) + jobs[i].need_time;

    cout << setw(10) << jobs[i].number << setw(12) << jobs[i].reach_time << setw(12) << jobs[i].start_time << setw(14) << jobs[i].wait_time << endl;

    current_hour = current_hour + (jobs[i].need_time + current_minute) / 60;
    current_minute = (jobs[i].need_time + current_minute) % 60;

    total_time += jobs[i].wait_time;
  }

  cout << endl
       << "总周转时间:" << total_time << "   平均周转时间:" << total_time / quantity << endl;
}

//运算时间短的作业优先算法
void shorter()
{
  int i, j, p;
  int current_hour;
  int current_minute;
  int current_need_time;

  int total_time = 0;

  //输出作业流
  cout << endl;
  cout << endl
       << "时间短作业优先算法作业流(开始调度时刻为最后一个作业到达系统的时间)" << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14) << "所需时间(分)" << setw(12) << "开始时间" << setw(14) << "周转时间(分)" << endl;

  current_hour = jobs[quantity - 1].reach_hour;
  current_minute = jobs[quantity - 1].reach_minite;
  for (i = 0; i < quantity; i++)
  {
    current_need_time = 30000;
    for (j = 0; j < quantity; j++)
    {
      if ((jobs[j].visited == 0) && (jobs[j].need_time < current_need_time))
      {
        p = j;
        current_need_time = jobs[j].need_time;
      }
    }

    jobs[p].start_time = current_hour * 100 + current_minute;
    jobs[p].wait_time = (current_hour - jobs[p].reach_hour) * 60 + (current_minute - jobs[p].reach_minite) + jobs[p].need_time;

    cout << setw(10) << jobs[p].number << setw(12) << jobs[p].reach_time << setw(14) << jobs[p].need_time << setw(12) << jobs[p].start_time << setw(14) << jobs[p].wait_time << endl;

    current_hour = current_hour + (jobs[p].need_time + current_minute) / 60;
    current_minute = (jobs[p].need_time + current_minute) % 60;

    jobs[p].visited = 1;

    total_time += jobs[p].wait_time;
  }
  cout << endl
       << "总周转时间:" << total_time << "   平均周转时间:" << total_time / quantity << endl;
}

//优先数调度算法
void privilege()
{
  int i, j, p;
  int current_hour;
  int current_minute;
  int current_privilege;

  int total_time = 0;

  //输出作业流
  cout << endl;
  cout << endl
       << "优先数调度算法作业流(开始调度时刻为最后一个作业到达系统的时间)" << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(14) << "优先级(0>1)" << setw(12) << "开始时间" << setw(14) << "周转时间(分)" << endl;

  current_hour = jobs[quantity - 1].reach_hour;
  current_minute = jobs[quantity - 1].reach_minite;
  for (i = 0; i < quantity; i++)
  {
    current_privilege = 30000;
    for (j = 0; j < quantity; j++)
    {
      if ((jobs[j].visited == 0) && (jobs[j].privilege < current_privilege))
      {
        p = j;
        current_privilege = jobs[j].privilege;
      }
    }
    jobs[p].start_time = current_hour * 100 + current_minute;
    jobs[p].wait_time = (current_hour - jobs[p].reach_hour) * 60 + (current_minute - jobs[p].reach_minite) + jobs[p].need_time;

    cout << setw(10) << jobs[p].number << setw(12) << jobs[p].reach_time << setw(14) << jobs[p].privilege << setw(12) << jobs[p].start_time << setw(14) << jobs[p].wait_time << endl;

    current_hour = current_hour + (jobs[p].need_time + current_minute) / 60;
    current_minute = (jobs[p].need_time + current_minute) % 60;

    jobs[p].visited = 1;

    total_time += jobs[p].wait_time;
  }
  cout << endl
       << "总周转时间:" << total_time << "   平均周转时间:" << total_time / quantity << endl;
}

//响应比最高者优先调度算法
void excellent()
{
  int i, j, p;
  int current_hour;
  int current_minute;
  float current_excellent;

  int total_time = 0;

  //输出作业流
  cout << endl;
  cout << endl
       << "响应比高者优先调度算法作业流(开始调度时刻为最后一个作业到达系统的时间)" << endl;
  cout << "------------------------------------------------------------------------" << endl;
  cout.setf(2);
  cout << setw(10) << "作业号" << setw(12) << "到达时间" << setw(12) << "开始时间" << setw(14) << "周转时间(分)" << endl;

  current_hour = jobs[quantity - 1].reach_hour;
  current_minute = jobs[quantity - 1].reach_minite;
  for (i = 0; i < quantity; i++)
  {
    current_excellent = -1;
    for (j = 0; j < quantity; j++)
    {
      if (jobs[j].visited == 0)
      {
        jobs[j].wait_time = (current_hour - jobs[j].reach_hour) * 60 + (current_minute - jobs[j].reach_minite);
        jobs[j].excellent = (float)(jobs[j].wait_time / jobs[j].need_time);
      }
    }

    for (j = 0; j < quantity; j++)
    {
      if ((jobs[j].visited == 0) && (jobs[j].excellent > current_excellent))
      {
        p = j;
        current_excellent = jobs[j].excellent;
      }
    }
    jobs[p].start_time = current_hour * 100 + current_minute;
    jobs[p].wait_time = (current_hour - jobs[p].reach_hour) * 60 + (current_minute - jobs[p].reach_minite) + jobs[p].need_time;

    cout << setw(10) << jobs[p].number << setw(12) << jobs[p].reach_time << setw(12) << jobs[p].start_time << setw(14) << jobs[p].wait_time << endl;

    current_hour = current_hour + (jobs[p].need_time + current_minute) / 60;
    current_minute = (jobs[p].need_time + current_minute) % 60;

    jobs[p].visited = 1;

    total_time += jobs[p].wait_time;
  }
  cout << endl
       << "总周转时间:" << total_time << "   平均周转时间:" << total_time / quantity << endl;
}

int main()
{
  initial();

  readData();

  FIFO();

  shorter();
  reset();

  privilege();
  reset();

  excellent();

  return 0;
}
