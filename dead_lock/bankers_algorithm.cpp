/*
银行家算法

银行家可以把一定数量的资金供多个用户周转使用，为保证资金的安全，银行家规定：

1.当一个用户对资金的最大需求量不超过银行家现有的资金就要接纳该用户;
2.用户可以分期贷款，但贷款的总数不能超过最大需求量;
3.当银行家现有的资金不能满足用户向需的贷款数时，对用户的贷款可推迟支付,但总能使用户在有限的时间里得到贷款;
4.当用户得到所需的全部资金后,一定能在有限的时间里归还所有的资金.

假设共有3类资源A B C,分别为10,5,7个
进程列表存在文件中，格式为

进程名 对A类资源需求数量  对B类资源需求数量  对c资源需求数量

输出每个进程的处理结果


本程序包括:银行家算法
*/

#include <iomanip>
#include <iostream>

using namespace std;

const int MAX_PROCESS = 20;
const int RESOURCE_A_SIZE = 10; //定义A类资源的数量
const int RESOURCE_B_SIZE = 5;
const int RESOURCE_C_SIZE = 7;

typedef struct node {
  int allocated_a;
  int allocated_b;
  int allocated_c;
  int remained_a;
  int remained_b;
  int remained_c;
} bank;

typedef struct node1 {
  char name[20];
  int allocated_a;
  int allocated_b;
  int allocated_c;
  int remained_a;
  int remained_b;
  int remained_c;
} process;

bank banker;
process processes[MAX_PROCESS];
int process_size;

//初始化函数
void init() {
  int i;

  banker.allocated_a = RESOURCE_A_SIZE;
  banker.allocated_b = RESOURCE_B_SIZE;
  banker.allocated_c = RESOURCE_C_SIZE;
  banker.remained_a = RESOURCE_A_SIZE;
  banker.remained_b = RESOURCE_B_SIZE;
  banker.remained_c = RESOURCE_C_SIZE;

  for (i = 0; i < MAX_PROCESS; i++) {
    strcpy(processes[i].name, "");
    processes[i].allocated_a = 0;
    processes[i].allocated_b = 0;
    processes[i].allocated_c = 0;
    processes[i].remained_a = 0;
    processes[i].remained_b = 0;
    processes[i].remained_c = 0;
  }
}

//新加作业
void add_task() {
  char name[20];
  int flag = 0;
  int t;
  int resource_a, resource_b, resource_c;
  int i;

  cout << endl;
  cout << "新加作业" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "请输入新加作业名:";
  cin >> name;

  for (i = 0; i < process_size; i++) {
    if (!strcmp(processes[i].name, name)) {
      flag = 1;
      break;
    }
  }

  if (flag) {
    cout << "错误,作业已存在" << endl;
  } else {
    cout << "本作业所需A类资源:";
    cin >> resource_a;
    cout << "本作业所需B类资源:";
    cin >> resource_b;
    cout << "本作业所需C类资源:";
    cin >> resource_c;

    t = 1;
    cout << resource_a << banker.remained_a;
    if (resource_a > banker.remained_a) {
      cout << "错误,所需A类资源大于银行家所剩A类资源" << endl;
      t = 0;
    }
    if (resource_b > banker.remained_b) {
      cout << "错误,所需B类资源大于银行家所剩B类资源" << endl;
      t = 0;
    }
    if (resource_c > banker.remained_c) {
      cout << "错误,所需C类资源大于银行家所剩C类资源" << endl;
      t = 0;
    }
    if (t) {
      strcpy(processes[process_size].name, name);
      processes[process_size].remained_a = resource_a;
      processes[process_size].remained_b = resource_b;
      processes[process_size].remained_c = resource_c;

      process_size++;
      cout << "新加作业成功" << endl;
    } else {
      cout << "新加作业失败" << endl;
    }
  }
}

//为作业申请资源
void alloc() {
  char name[20];
  int i, p;
  int a, b, c;
  int flag;

  cout << endl << "为作业申请资源" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "要申请资源的作业名:";
  cin >> name;

  p = -1;
  for (i = 0; i < process_size; i++) {
    if (!strcmp(processes[i].name, name)) {
      p = i;
      break;
    }
  }
  if (p != -1) {
    cout << "该作业要申请A类资源数量:";
    cin >> a;
    cout << "该作业要申请B类资源数量:";
    cin >> b;
    cout << "该作业要申请C类资源数量:";
    cin >> c;

    flag = 1;
    if ((a > banker.remained_a) ||
        (a > processes[p].remained_a - processes[p].allocated_a)) {
      cout << "错误,所申请A类资源大于银行家所剩A类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if ((b > banker.remained_b) ||
        (b > processes[p].remained_b - processes[p].allocated_b)) {
      cout << "错误,所申请B类资源大于银行家所剩B类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if ((c > banker.remained_c) ||
        (c > processes[p].remained_c - processes[p].allocated_c)) {
      cout << "错误,所申请C类资源大于银行家所剩C类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if (flag) {
      banker.remained_a -= a;
      banker.remained_b -= b;
      banker.remained_c -= c;

      processes[p].allocated_a += a;
      processes[p].allocated_b += b;
      processes[p].allocated_c += c;

      cout << "为作业申请资源成功" << endl;
    } else {
      cout << "为作业申请资源失败" << endl;
    }
  } else {
    cout << "该作业不存在" << endl;
  }
}

//撤消作业
void release() {
  char name[20];
  int i, p;

  cout << endl << "撤消作业" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "要撤消作业名:";
  cin >> name;

  p = -1;
  for (i = 0; i < process_size; i++) {
    if (!strcmp(processes[i].name, name)) {
      p = i;
      break;
    }
  }
  if (p != -1) {
    banker.remained_a += processes[p].allocated_a;
    banker.remained_b += processes[p].allocated_b;
    banker.remained_c += processes[p].allocated_c;
    for (i = p; i < process_size - 1; i++) {
      processes[i] = processes[i + 1];
    }
    strcpy(processes[process_size - 1].name, "");
    processes[process_size - 1].allocated_a = 0;
    processes[process_size - 1].allocated_b = 0;
    processes[process_size - 1].allocated_c = 0;
    processes[process_size - 1].remained_a = 0;
    processes[process_size - 1].remained_b = 0;
    processes[process_size - 1].remained_c = 0;

    process_size--;

    cout << "撤消作业成功" << endl;
  } else {
    cout << "撤消作业失败" << endl;
  }
}

//查看资源情况
void view() {
  int i;

  cout << endl << "查看资源情况" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "银行家所剩资源(剩余资源/总共资源)" << endl;
  cout << "A类:" << banker.remained_a << "/" << banker.allocated_a;
  cout << "   B类:" << banker.remained_b << "/" << banker.allocated_b;
  cout << "   C类:" << banker.remained_c << "/" << banker.allocated_c;

  cout << endl << endl << "作业占用情况(已占用资源/所需资源)" << endl << endl;
  if (process_size > 0) {
    for (i = 0; i < process_size; i++) {
      cout << "作业名:" << processes[i].name << endl;
      cout << "A类:" << processes[i].allocated_a << "/"
           << processes[i].remained_a;
      cout << "   B类:" << processes[i].allocated_b << "/"
           << processes[i].remained_b;
      cout << "   C类:" << processes[i].allocated_c << "/"
           << processes[i].remained_c;
      cout << endl;
    }
  } else {
    cout << "当前没有作业" << endl;
  }
}

int main() {
  int chioce;
  int flag = 1;

  init();

  while (flag) {
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "1.新加作业  2.为作业申请资源  3.撤消作业" << endl;
    cout << "4.查看资源情况   0.退出系统" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce) {
    case 1:
      add_task();
      break;
    case 2:
      alloc();
      break;
    case 3:
      release();
      break;
    case 4:
      view();
      break;
    case 0:
      flag = 0;
      break;
    default:
      cout << "选择错误" << endl << endl;
    }
  }
  return 0;
}
