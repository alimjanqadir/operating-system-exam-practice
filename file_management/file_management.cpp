/*
四、 文件管理
文件管理的试题比较多，主要就是模拟操作系统中的 建立文件、打开文件、读文件、写文件、、关闭文件、 、删除文件、、建立目录、、
显示目录内容、显示文件内容、、改变文件属性等操作。大家可以参考书本253页的上机指导。

北大2001年试题：
建立一个树型文件目录
假设程序启动运行后在根目录下且根目录为空。

实习检查：
1、运行程序，由检查教师给出文件名，该文件中存有相应的若干命令。（程序应做提示，界面友好）。
2、要求实现两个命令：
mkdir 目录名（目录已存在，应给出错误信息。）
cd 目录名（目录不存在，应给出错误信息。）
3、你所编制的程序应读入文件，并执行其中的每一条命令。
4、在屏幕上显示文件目录的结构。（界面自己设计，但要清晰明了。）

2002年北京大学的试题：
操作系统上机考试题
题目：模拟文件系统
要求：模拟一个文件系统，包括目录文件，普通文件，并实现对它们的一些
基本操作。
假定每个目录文件最多只能占用一个块；一个目录项包括文件名（下一级目录
名），文件类型，文件长度，指向文件内容（下一级目录）的指针内容。普通文件可以
只用目录项（FCB）代表。（详细的数据结构见后面的说明）
程序功能方面的要求：
需要实现一个命令行操作界面，包含如下命令：
1 改变目录
格式：CD〈目录名〉
功能：工作目录转移到指定的目录下，只要求完成改变到当前目录的某一个子目录
下的功能，不要求实现相对目录以及绝对目录。
2 创建文件
格式：CREATE〈文件名〉〈文件长度〉
功能：创立一个指定名字的新文件，即在目录中增加一项，不考虑文件内容，但必
须能输入文件长度。
3 删除文件
格式：DEL〈希望删除的文件名〉
功能：删除指定的文件
4 显示目录
格式：LSALL
功能：显示全部目录以及文件，输出时要求先输出接近根的目录，再输出子目录。
图示如图。
5 创建目录
格式：MD〈目录名〉
功能：在当前路径下创建指定的目录
6 删除目录
格式：RD〈目录名〉
功能：删除当前目录下的指定目录，如果该目录为空，则可删除，否则应提示是否
作删除，删除操作将该目录下的全部文件和子目录都删除。
对于上述功能要求，完成1-4为及格，完成1-5为良，完成1-6为优。

程序实现方面的要求：
1 对于重名（创建时），文件不存在（删除时），目录不存在（改变目录时）等错误*
作情况，程序应该作出相应处理并给出错误信息，但是程序不得因此而退出。
2 界面友好，程序强壮。
3 界面的提示符为#，提示的命令以及调试的方法应和前面的要求一致。不要自己设计命
令或者附加不要求的功能。
4 在考卷的说明部分（背面）有一段程序的源代码以及对源代码的说明，考试的编码应
在这个程序的基础上修改而成。这段源代码中规定了文件系统使用的数据结构和需要实
现的函数框架，请将你的实现代码填写到合适的位置中去，可以自己添加辅助数据结构、
变量、常量以及函数，但是不得改变已有的代码（如数据结构的定义以及函数的名称以
及参数说明）。
5 考试提交的源程序请命名为filesys.c。
6 程序设计环境使用TC2.0，在DOS操作系统下完成全部程序代码。

  本程序包括实现:

  1 改变目录
格式：CD〈目录名〉
功能：工作目录转移到指定的目录下，只要求完成改变到当前目录的某一个子目录
下的功能，不要求实现相对目录以及绝对目录。
2 创建文件
格式：CREATE〈文件名〉〈文件长度〉
功能：创立一个指定名字的新文件，即在目录中增加一项，不考虑文件内容，但必
须能输入文件长度。
3 删除文件
格式：DEL〈希望删除的文件名〉
功能：删除指定的文件
4 显示目录
格式：LSALL
功能：显示全部目录以及文件，输出时要求先输出接近根的目录，再输出子目录。
图示如图。
5 创建目录
格式：MD〈目录名〉
功能：在当前路径下创建指定的目录
6 删除目录
格式：RD〈目录名〉

  VC++调试通过

  (C)copyright by Neo

  欢迎大家测试 请问题请Email:sony006@163.com
*/

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

//  ******************目录和文件的结构定义******
typedef struct node
{
  char name[50];       /*目录或文件的名字*/
  int type;            /*0代表目录，1代表普通文件*/
  struct node *next;   /*指向下一个兄弟结点的指针*/
  struct node *sub;    /*指向第一个子结点的指针*/
  struct node *father; /*指向父结点的指针*/

  int size; /*如果是文件则表示文件的大小*/

} dirNode;

dirNode *workDir; //定义当前工作目录
dirNode root;     //定义根目录

char path[100]; //定义路径信息

//初始化函数
void initial()
{
  strcpy(root.name, "root");
  root.type = 0;
  root.next = NULL;
  root.sub = NULL;
  root.father = NULL;
  root.size = 0;

  workDir = &root;

  strcpy(path, "root");
}

//初始化新结点函数
dirNode *init()
{
  dirNode *p;

  p = new dirNode;

  strcpy(root.name, "");
  root.type = 0;
  root.next = NULL;
  root.sub = NULL;
  root.father = NULL;
  root.size = 0;

  return p;
}

// 1 改变目录
void CD(char dirName[])
{
  dirNode *p;
  int flag = 0;

  p = workDir->sub;
  if (p == NULL)
  {
    cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
  }
  else
  {
    while (p)
    {
      if (p->type == 0)
      {
        if (!strcmp(p->name, dirName))
        {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1)
    {
      workDir = p;
      strcat(path, "\\ ");
      strcat(path, p->name);
      cout << "工作目录已进入\"" << dirName << "\"" << endl;
    }
    else
    {
      cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
    }
  }
}

// 2 创建文件
void CREATE(char fileName[], int fileSize)
{
  int flag;
  dirNode *p, *q;

  q = new dirNode;
  strcpy(q->name, fileName);
  q->sub = NULL;
  q->type = 1;
  q->next = NULL;
  q->father = workDir;
  q->size = fileSize;

  p = workDir->sub;

  if (p == NULL)
  {
    workDir->sub = q;
    cout << "\"" << fileName << "\"文件创建成功" << endl;
  }
  else
  {
    flag = 0;
    while (p)
    {
      if (p->type == 1)
      {
        if (!strcmp(p->name, fileName))
        {
          flag = 1;
          cout << "错误,\"" << fileName << "\"文件已存在" << endl;
        }
      }
      p = p->next;
    }
    if (flag == 0)
    {
      p = workDir->sub;
      while (p->next)
      {
        p = p->next;
      }
      p->next = q;
      cout << "\"" << fileName << "\"文件创建成功" << endl;
    }
  }
}

// 3 删除文件
void DEL(char fileName[])
{
  dirNode *p, *q;
  int flag = 0;

  p = workDir->sub;
  if (p == NULL)
  {
    cout << "错误,\"" << fileName << "\"文件不存在" << endl;
  }
  else
  {
    while (p)
    {
      if (p->type == 1)
      {
        if (!strcmp(p->name, fileName))
        {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1)
    {
      if (p == workDir->sub)
      {
        workDir->sub = p->next;
      }
      else
      {
        q = workDir->sub;
        while (q->next != p)
        {
          q = q->next;
        }
        q->next = p->next;
        delete p;
      }
      cout << "\"" << fileName << "\"文件已删除" << endl;
    }
    else
    {
      cout << "错误,\"" << fileName << "\"文件不存在" << endl;
    }
  }
}

// 4 显示所有目录

//显示本目录下所有兄弟目录和文件
void dir(dirNode *p)
{
  while (p)
  {
    if (p->type == 0)
    {
      cout.setf(2);
      cout << setw(14) << p->name << setw(12) << "<DIR>" << endl;
    }
    else
    {
      cout.setf(2);
      cout << setw(14) << p->name << setw(12) << "<FILE>" << setw(10) << p->size << endl;
    }
    p = p->next;
  }
}
//
void dirs(dirNode *p, const char str[])
{
  char newstr[100];
  dirNode *q;
  cout << str << "下子目录及文件:" << endl;
  dir(p);

  q = p;
  if (q->sub)
  {
    strcpy(newstr, "");
    strcat(newstr, str);
    strcat(newstr, "\\");
    strcat(newstr, q->name);

    dirs(q->sub, newstr);
  }
  q = p;
  while (q->next)
  {
    if (q->next->sub)
    {
      strcpy(newstr, "");
      strcat(newstr, str);
      strcat(newstr, " \\");
      strcat(newstr, q->next->name);

      dirs(q->next->sub, newstr);
    }
    q = q->next;
  }
}

// 显示所有目录
void LSALL()
{
  dirNode *p;

  cout << "显示所有目录结构" << endl
       << "--------------------------------------------" << endl;

  p = root.sub;

  dirs(p, "root");
}
// 5 创建目录
void MD(char dirName[])
{
  int flag;
  dirNode *p, *q;

  q = new dirNode;
  strcpy(q->name, dirName);
  q->sub = NULL;
  q->type = 0;
  q->next = NULL;
  q->father = workDir;
  q->size = 0;

  p = workDir->sub;

  if (p == NULL)
  {
    workDir->sub = q;
    cout << "\"" << dirName << "\"子目录创建成功" << endl;
  }
  else
  {
    flag = 0;
    while (p)
    {
      if (p->type == 0)
      {
        if (!strcmp(p->name, dirName))
        {
          flag = 1;
          cout << "错误,\"" << dirName << "\"子目录已存在" << endl;
        }
      }
      p = p->next;
    }
    if (flag == 0)
    {
      p = workDir->sub;
      while (p->next)
      {
        p = p->next;
      }
      p->next = q;
      cout << "\"" << dirName << "\"子目录创建成功" << endl;
    }
  }
}

// 6 删除目录
void RD(char dirName[])
{
  /*
  删除当前目录下的指定目录，如果该目录为空，则可删除，否则应提示是否
  作删除，删除操作将该目录下的全部文件和子目录都删除

    这题目有点问题，因为CD只要求完成改变到当前目录的某一个子目录
  下的功能，不要求实现相对目录以及绝对目录。所以不进入子目录是不可能在子目录下再建子目录或文件的。
  因此只要实现删除工作目录的子目录就可以了，不要去管“如果该目录为空，则可删除，否则应提示是否
  作删除，删除操作将该目录下的全部文件和子目录都删除”。
  */
  dirNode *p, *q;
  int flag = 0;

  p = workDir->sub;
  if (p == NULL)
  {
    cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
  }
  else
  {
    while (p)
    {
      if (p->type == 0)
      {
        if (!strcmp(p->name, dirName))
        {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1)
    {
      if (p == workDir->sub)
      {
        workDir->sub = p->next;
      }
      else
      {
        q = workDir->sub;
        while (q->next != p)
        {
          q = q->next;
        }
        q->next = p->next;
        delete p;
      }
      cout << "\"" << dirName << "\"子目录已删除" << endl;
    }
    else
    {
      cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
    }
  }
}

int main()
{
  int len, fileSize, flag, comm;

  char string[20];
  char command[10];
  char name[40];
  char *s, *s1;

  initial();

  flag = 1;
  while (flag)
  {
    cout << endl
         << "        模拟文件管理模拟系统" << endl;
    cout << endl
         << "CD 改变目录     CREATE 创建文件 DEL 删除文件 " << endl
         << "LSALL 显示目录  MD 创建目录     RD 删除目录" << endl
         << "exit 退出" << endl;
    cout << endl
         << "-----------------------------------------------" << endl;
    printf("%s:>#", path);
    gets(string);

    len = strlen(string);
    if (len == 0)
    {
      strcpy(command, "errer");
    }
    else
    {
      //获得命令
      s = NULL;
      s = strchr(string, ' ');
      if (s != NULL)
      {
        *s = '\0';
      }
      strcpy(command, string);

      //测试命令类型
      if ((!strcmp(command, "CD")) || !strcmp(command, "cd"))
      {
        comm = 1;
      }
      else
      {
        if ((!strcmp(command, "CREATE")) || !strcmp(command, "create"))
        {
          comm = 2;
        }
        else
        {
          if ((!strcmp(command, "DEL")) || !strcmp(command, "del"))
          {
            comm = 3;
          }
          else
          {
            if ((!strcmp(command, "LSALL")) || !strcmp(command, "lsall"))
            {
              comm = 4;
            }
            else
            {
              if ((!strcmp(command, "MD")) || !strcmp(command, "md"))
              {
                comm = 5;
              }
              else
              {
                if ((!strcmp(command, "RD")) || !strcmp(command, "rd"))
                {
                  comm = 6;
                }
                else
                {
                  if ((!strcmp(command, "EXIT")) || !strcmp(command, "exit"))
                  {
                    comm = 0;
                  }
                  else
                  {
                    comm = 100;
                  }
                }
              }
            }
          }
        }
      }
      switch (comm)
      {
      case 1:
        // 1 改变目录
        strcpy(name, s + 1);
        CD(name);
        break;
      case 2:
        // 2 创建文件
        s1 = strchr(s + 1, ' ');
        *s1 = '\0';
        strcpy(name, s + 1);
        fileSize = atoi(s1 + 1);
        CREATE(name, 0);
        break;
      case 3:
        // 3 删除文件
        strcpy(name, s + 1);
        DEL(name);
        break;
      case 4:
        // 4 显示目录
        LSALL();
        break;
      case 5:
        // 5 创建目录
        strcpy(name, s + 1);
        MD(name);
        break;
      case 6:
        // 6 删除目录
        strcpy(name, s + 1);
        RD(name);
        break;
      case 0:
        // 0 退出系统
        flag = 0;
        break;
      default:
        cout << "命令错误" << endl;
      }
    }
  }
}
