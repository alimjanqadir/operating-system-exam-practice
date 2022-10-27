/*
银行家算法

银行家可以把一定数量的资金供多个用户周转使用，为保证资金的安全银行家规定：

1.当一个用户对资金的最大需求量不超过银行家现有的资金就要接纳该用户;
2.用户可以分期贷款，但贷款的总数不能超过最大需求量;
3.当银行家现有的资金不能满足用户向需的贷款数时,对用户的贷款可推迟支付,但总能使用户在有限的时间里得到贷款;
4.当用户得到所需的全部资金后,一定能在有限的时间里归还所有的资金.

假设共有3类资源A B C,分别为10,5,7个
进程列表存在文件中,格式为

进程名 对A类资源需求数量  对B类资源需求数量  对c资源需求数量

输出每个进程的处理结果


本程序包括:银行家算法


  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include <iostream>
#include <iomanip>

using namespace std;

const int MAX_P = 20;
const int MAXA = 10; //定义A类资源的数量
const int MAXB = 5;
const int MAXC = 7;

typedef struct node
{
  int a;
  int b;
  int c;
  int remain_a;
  int remain_b;
  int remain_c;
} bank;

typedef struct node1
{
  char name[20];
  int a;
  int b;
  int c;
  int need_a;
  int need_b;
  int need_c;
} process;

bank banker;
process processes[MAX_P];
int quantity;

//初始化函数
void initial()
{
  int i;

  banker.a = MAXA;
  banker.b = MAXB;
  banker.c = MAXC;
  banker.remain_a = MAXA;
  banker.remain_b = MAXB;
  banker.remain_c = MAXC;

  for (i = 0; i < MAX_P; i++)
  {
    strcpy(processes[i].name, "");
    processes[i].a = 0;
    processes[i].b = 0;
    processes[i].c = 0;
    processes[i].need_a = 0;
    processes[i].need_b = 0;
    processes[i].need_c = 0;
  }
}

//新加作业
void add()
{
  char name[20];
  int flag = 0;
  int t;
  int need_a, need_b, need_c;
  int i;

  cout << endl;
  cout << "新加作业" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "请输入新加作业名:";
  cin >> name;

  for (i = 0; i < quantity; i++)
  {
    if (!strcmp(processes[i].name, name))
    {
      flag = 1;
      break;
    }
  }

  if (flag)
  {
    cout << "错误,作业已存在" << endl;
  }
  else
  {
    cout << "本作业所需A类资源:";
    cin >> need_a;
    cout << "本作业所需B类资源:";
    cin >> need_b;
    cout << "本作业所需C类资源:";
    cin >> need_c;

    t = 1;
    cout << need_a << banker.remain_a;
    if (need_a > banker.remain_a)
    {
      cout << "错误,所需A类资源大于银行家所剩A类资源" << endl;
      t = 0;
    }
    if (need_b > banker.remain_b)
    {
      cout << "错误,所需B类资源大于银行家所剩B类资源" << endl;
      t = 0;
    }
    if (need_c > banker.remain_c)
    {
      cout << "错误,所需C类资源大于银行家所剩C类资源" << endl;
      t = 0;
    }
    if (t)
    {
      strcpy(processes[quantity].name, name);
      processes[quantity].need_a = need_a;
      processes[quantity].need_b = need_b;
      processes[quantity].need_c = need_c;

      quantity++;
      cout << "新加作业成功" << endl;
    }
    else
    {
      cout << "新加作业失败" << endl;
    }
  }
}

//为作业申请资源
void bid()
{
  char name[20];
  int i, p;
  int a, b, c;
  int flag;

  cout << endl
       << "为作业申请资源" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "要申请资源的作业名:";
  cin >> name;

  p = -1;
  for (i = 0; i < quantity; i++)
  {
    if (!strcmp(processes[i].name, name))
    {
      p = i;
      break;
    }
  }
  if (p != -1)
  {
    cout << "该作业要申请A类资源数量:";
    cin >> a;
    cout << "该作业要申请B类资源数量:";
    cin >> b;
    cout << "该作业要申请C类资源数量:";
    cin >> c;

    flag = 1;
    if ((a > banker.remain_a) || (a > processes[p].need_a - processes[p].a))
    {
      cout << "错误,所申请A类资源大于银行家所剩A类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if ((b > banker.remain_b) || (b > processes[p].need_b - processes[p].b))
    {
      cout << "错误,所申请B类资源大于银行家所剩B类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if ((c > banker.remain_c) || (c > processes[p].need_c - processes[p].c))
    {
      cout << "错误,所申请C类资源大于银行家所剩C类资源或该进程还需数量" << endl;
      flag = 0;
    }
    if (flag)
    {
      banker.remain_a -= a;
      banker.remain_b -= b;
      banker.remain_c -= c;

      processes[p].a += a;
      processes[p].b += b;
      processes[p].c += c;

      cout << "为作业申请资源成功" << endl;
    }
    else
    {
      cout << "为作业申请资源失败" << endl;
    }
  }
  else
  {
    cout << "该作业不存在" << endl;
  }
}

//撤消作业
void finished()
{
  char name[20];
  int i, p;

  cout << endl
       << "撤消作业" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "要撤消作业名:";
  cin >> name;

  p = -1;
  for (i = 0; i < quantity; i++)
  {
    if (!strcmp(processes[i].name, name))
    {
      p = i;
      break;
    }
  }
  if (p != -1)
  {
    banker.remain_a += processes[p].a;
    banker.remain_b += processes[p].b;
    banker.remain_c += processes[p].c;
    for (i = p; i < quantity - 1; i++)
    {
      processes[i] = processes[i + 1];
    }
    strcpy(processes[quantity - 1].name, "");
    processes[quantity - 1].a = 0;
    processes[quantity - 1].b = 0;
    processes[quantity - 1].c = 0;
    processes[quantity - 1].need_a = 0;
    processes[quantity - 1].need_b = 0;
    processes[quantity - 1].need_c = 0;

    quantity--;

    cout << "撤消作业成功" << endl;
  }
  else
  {
    cout << "撤消作业失败" << endl;
  }
}

//查看资源情况
void view()
{
  int i;

  cout << endl
       << "查看资源情况" << endl;
  cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
  cout << "银行家所剩资源(剩余资源/总共资源)" << endl;
  cout << "A类:" << banker.remain_a << "/" << banker.a;
  cout << "   B类:" << banker.remain_b << "/" << banker.b;
  cout << "   C类:" << banker.remain_c << "/" << banker.c;

  cout << endl
       << endl
       << "作业占用情况(已占用资源/所需资源)" << endl
       << endl;
  if (quantity > 0)
  {
    for (i = 0; i < quantity; i++)
    {
      cout << "作业名:" << processes[i].name << endl;
      cout << "A类:" << processes[i].a << "/" << processes[i].need_a;
      cout << "   B类:" << processes[i].b << "/" << processes[i].need_b;
      cout << "   C类:" << processes[i].c << "/" << processes[i].need_c;
      cout << endl;
    }
  }
  else
  {
    cout << "当前没有作业" << endl;
  }
}

int main()
{
  int chioce;
  int flag = 1;

  initial();

  while (flag)
  {
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "1.新加作业  2.为作业申请资源  3.撤消作业" << endl;
    cout << "4.查看资源情况   0.退出系统" << endl;
    cout << "请选择:";
    cin >> chioce;

    switch (chioce)
    {
    case 1:
      add();
      break;
    case 2:
      bid();
      break;
    case 3:
      finished();
      break;
    case 4:
      view();
      break;
    case 0:
      flag = 0;
      break;
    default:
      cout << "选择错误" << endl
           << endl;
    }
  }
  return 0;
}
