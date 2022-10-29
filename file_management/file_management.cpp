/*
四、 文件管理
文件管理的试题比较多，主要就是模拟操作系统中的
建立文件、打开文件、读文件、写文件、关闭文件、删除文件、建立目录、
显示目录内容、显示文件内容、改变文件属性等操作。大家可以参考书本253页的上机指导。

北大2001年试题：
建立一个树型文件目录
假设程序启动运行后在根目录下且根目录为空。

实习检查：
1.运行程序，由检查教师给出文件名，该文件中存有相应的若干命令。（程序应做提示，界面友好）。
2.要求实现两个命令：
mkdir 目录名（目录已存在，应给出错误信息。）
cd 目录名（目录不存在，应给出错误信息。）
3.你所编制的程序应读入文件，并执行其中的每一条命令。
4.在屏幕上显示文件目录的结构。（界面自己设计，但要清晰明了。）

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
1.改变目录
格式：CD〈目录名〉
功能：工作目录转移到指定的目录下，只要求完成改变到当前目录的某一个子目录
下的功能，不要求实现相对目录以及绝对目录。
2.创建文件
格式：CREATE〈文件名〉〈文件长度〉
功能：创立一个指定名字的新文件，即在目录中增加一项，不考虑文件内容，但必
须能输入文件长度。
3.删除文件
格式：DEL〈希望删除的文件名〉
功能：删除指定的文件
4.显示目录
格式：LSALL
功能：显示全部目录以及文件，输出时要求先输出接近根的目录，再输出子目录。
图示如图。
5.创建目录
格式：MD〈目录名〉
功能：在当前路径下创建指定的目录
6.删除目录
格式：RD〈目录名〉
功能：删除当前目录下的指定目录，如果该目录为空，则可删除，否则应提示是否
作删除，删除操作将该目录下的全部文件和子目录都删除。
对于上述功能要求，完成1-4为及格，完成1-5为良，完成1-6为优。

程序实现方面的要求：
1.对于重名（创建时），文件不存在（删除时），目录不存在（改变目录时）等错误*
作情况，程序应该作出相应处理并给出错误信息，但是程序不得因此而退出。
2.界面友好，程序强壮。
3.界面的提示符为#，提示的命令以及调试的方法应和前面的要求一致。不要自己设计命
令或者附加不要求的功能。
4.在考卷的说明部分（背面）有一段程序的源代码以及对源代码的说明，考试的编码应
在这个程序的基础上修改而成。这段源代码中规定了文件系统使用的数据结构和需要实
现的函数框架，请将你的实现代码填写到合适的位置中去，可以自己添加辅助数据结构、
变量、常量以及函数，但是不得改变已有的代码（如数据结构的定义以及函数的名称以
及参数说明）。
5.考试提交的源程序请命名为filesys.c。
6.程序设计环境使用TC2.0，在DOS操作系统下完成全部程序代码。

本程序包括实现:

1.改变目录
格式：CD〈目录名〉
功能：工作目录转移到指定的目录下，只要求完成改变到当前目录的某一个子目录
下的功能，不要求实现相对目录以及绝对目录。
2.创建文件
格式：CREATE〈文件名〉〈文件长度〉
功能：创立一个指定名字的新文件，即在目录中增加一项，不考虑文件内容，但必
须能输入文件长度。
3.删除文件
格式：DEL〈希望删除的文件名〉
功能：删除指定的文件
4.显示目录
格式：LSALL
功能：显示全部目录以及文件，输出时要求先输出接近根的目录，再输出子目录。
图示如图。
5.创建目录
格式：MD〈目录名〉
功能：在当前路径下创建指定的目录
6.删除目录
格式：RD〈目录名〉
*/

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

//  ******************目录和文件的结构定义******
typedef struct node {
  char name[50];       /*目录或文件的名字*/
  int type;            /*0代表目录，1代表普通文件*/
  struct node *next;   /*指向下一个兄弟结点的指针*/
  struct node *child;  /*指向第一个子结点的指针*/
  struct node *parent; /*指向父结点的指针*/

  int size; /*如果是文件则表示文件的大小*/

} directory;

directory *work_dir; //定义当前工作目录
directory root;      //定义根目录

char path[100]; //定义路径信息

//初始化函数
void init() {
  strcpy(root.name, "root");
  root.type = 0;
  root.next = NULL;
  root.child = NULL;
  root.parent = NULL;
  root.size = 0;

  work_dir = &root;

  strcpy(path, "root");
}

//初始化新结点函数
directory *create_directory() {
  directory *p;

  p = new directory;

  strcpy(root.name, "");
  root.type = 0;
  root.next = NULL;
  root.child = NULL;
  root.parent = NULL;
  root.size = 0;

  return p;
}

// 1 改变目录
void CD(char dirName[]) {
  directory *p;
  int flag = 0;

  p = work_dir->child;
  if (p == NULL) {
    cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
  } else {
    while (p) {
      if (p->type == 0) {
        if (!strcmp(p->name, dirName)) {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1) {
      work_dir = p;
      strcat(path, "\\ ");
      strcat(path, p->name);
      cout << "工作目录已进入\"" << dirName << "\"" << endl;
    } else {
      cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
    }
  }
}

// 2 创建文件
void CREATE(char file_name[], int fileSize) {
  int flag;
  directory *p, *q;

  q = new directory;
  strcpy(q->name, file_name);
  q->child = NULL;
  q->type = 1;
  q->next = NULL;
  q->parent = work_dir;
  q->size = fileSize;

  p = work_dir->child;

  if (p == NULL) {
    work_dir->child = q;
    cout << "\"" << file_name << "\"文件创建成功" << endl;
  } else {
    flag = 0;
    while (p) {
      if (p->type == 1) {
        if (!strcmp(p->name, file_name)) {
          flag = 1;
          cout << "错误,\"" << file_name << "\"文件已存在" << endl;
        }
      }
      p = p->next;
    }
    if (flag == 0) {
      p = work_dir->child;
      while (p->next) {
        p = p->next;
      }
      p->next = q;
      cout << "\"" << file_name << "\"文件创建成功" << endl;
    }
  }
}

// 3 删除文件
void DEL(char file_name[]) {
  directory *p, *q;
  int flag = 0;

  p = work_dir->child;
  if (p == NULL) {
    cout << "错误,\"" << file_name << "\"文件不存在" << endl;
  } else {
    while (p) {
      if (p->type == 1) {
        if (!strcmp(p->name, file_name)) {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1) {
      if (p == work_dir->child) {
        work_dir->child = p->next;
      } else {
        q = work_dir->child;
        while (q->next != p) {
          q = q->next;
        }
        q->next = p->next;
        delete p;
      }
      cout << "\"" << file_name << "\"文件已删除" << endl;
    } else {
      cout << "错误,\"" << file_name << "\"文件不存在" << endl;
    }
  }
}

// 4 显示所有目录

//显示本目录下所有兄弟目录和文件
void dir(directory *p) {
  while (p) {
    if (p->type == 0) {
      cout.setf(2);
      cout << setw(14) << p->name << setw(12) << "<DIR>" << endl;
    } else {
      cout.setf(2);
      cout << setw(14) << p->name << setw(12) << "<FILE>" << setw(10) << p->size
           << endl;
    }
    p = p->next;
  }
}
//
void dirs(directory *p, const char str[]) {
  char dir_path[100];
  directory *q;
  cout << str << "下子目录及文件:" << endl;
  dir(p);

  q = p;
  if (q->child) {
    strcpy(dir_path, "");
    strcat(dir_path, str);
    strcat(dir_path, "\\");
    strcat(dir_path, q->name);

    dirs(q->child, dir_path);
  }
  q = p;
  while (q->next) {
    if (q->next->child) {
      strcpy(dir_path, "");
      strcat(dir_path, str);
      strcat(dir_path, " \\");
      strcat(dir_path, q->next->name);

      dirs(q->next->child, dir_path);
    }
    q = q->next;
  }
}

// 显示所有目录
void LSALL() {
  directory *p;

  cout << "显示所有目录结构" << endl
       << "--------------------------------------------" << endl;

  p = root.child;

  if (p == NULL) {
    cout << "目前没有任何文件" << endl;
    return;
  }

  dirs(p, "root");
}
// 5 创建目录
void MD(char dir_name[]) {
  int flag;
  directory *p, *q;

  q = new directory;
  strcpy(q->name, dir_name);
  q->child = NULL;
  q->type = 0;
  q->next = NULL;
  q->parent = work_dir;
  q->size = 0;

  p = work_dir->child;

  if (p == NULL) {
    work_dir->child = q;
    cout << "\"" << dir_name << "\"子目录创建成功" << endl;
  } else {
    flag = 0;
    while (p) {
      if (p->type == 0) {
        if (!strcmp(p->name, dir_name)) {
          flag = 1;
          cout << "错误,\"" << dir_name << "\"子目录已存在" << endl;
        }
      }
      p = p->next;
    }
    if (flag == 0) {
      p = work_dir->child;
      while (p->next) {
        p = p->next;
      }
      p->next = q;
      cout << "\"" << dir_name << "\"子目录创建成功" << endl;
    }
  }
}

// 6 删除目录
void RD(char dirName[]) {
  /*
  删除当前目录下的指定目录，如果该目录为空，则可删除，否则应提示是否
  作删除，删除操作将该目录下的全部文件和子目录都删除

    这题目有点问题，因为CD只要求完成改变到当前目录的某一个子目录
  下的功能，不要求实现相对目录以及绝对目录。所以不进入子目录是不可能在子目录下再建子目录或文件的。
  因此只要实现删除工作目录的子目录就可以了，不要去管“如果该目录为空，则可删除，否则应提示是否
  作删除，删除操作将该目录下的全部文件和子目录都删除”。
  */
  directory *p, *q;
  int flag = 0;

  p = work_dir->child;
  if (p == NULL) {
    cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
  } else {
    while (p) {
      if (p->type == 0) {
        if (!strcmp(p->name, dirName)) {
          flag = 1;
          break;
        }
      }
      p = p->next;
    }
    if (flag == 1) {
      if (p == work_dir->child) {
        work_dir->child = p->next;
      } else {
        q = work_dir->child;
        while (q->next != p) {
          q = q->next;
        }
        q->next = p->next;
        delete p;
      }
      cout << "\"" << dirName << "\"子目录已删除" << endl;
    } else {
      cout << "错误,\"" << dirName << "\"子目录不存在" << endl;
    }
  }
}

int main() {
  char name[40];
  char *s, *s1;

  init();

  int is_running = 1;
  while (is_running) {
    cout << endl << "        模拟文件管理模拟系统" << endl;
    cout << endl
         << "CD 改变目录     CREATE 创建文件 DEL 删除文件 " << endl
         << "LSALL 显示目录  MD 创建目录     RD 删除目录" << endl
         << "exit 退出" << endl;
    cout << endl << "-----------------------------------------------" << endl;
    printf("%s:>#", path);
    char input[50];
    fgets(input, 50, stdin);

    char command[50];
    if (strlen(input) == 0) {
      strcpy(command, "errer");
    } else {
      //获得命令
      char *space_pointer = strchr(input, ' ');
      char param[50];
      if (space_pointer != NULL) {
        int space_position = space_pointer - input;
        strcpy(command, input);
        command[space_position] = '\0';
        strcpy(param, space_pointer + 1);
        param[strlen(param) - 1] = '\0';
      } else {
        char *newline_pointer = strchr(input, '\n');
        *newline_pointer = '\0';
        strcpy(command, input);
      }

      //测试命令类型
      int command_type = -1;
      if ((!strcmp(command, "CD")) || !strcmp(command, "cd")) {
        command_type = 1;
      } else {
        if ((!strcmp(command, "CREATE")) || !strcmp(command, "create")) {
          command_type = 2;
        } else {
          if ((!strcmp(command, "DEL")) || !strcmp(command, "del")) {
            command_type = 3;
          } else {
            if ((!strcmp(command, "LSALL")) || !strcmp(command, "lsall")) {
              command_type = 4;
            } else {
              if ((!strcmp(command, "MD")) || !strcmp(command, "md")) {
                command_type = 5;
              } else {
                if ((!strcmp(command, "RD")) || !strcmp(command, "rd")) {
                  command_type = 6;
                } else {
                  if ((!strcmp(command, "EXIT")) || !strcmp(command, "exit")) {
                    command_type = 0;
                  } else {
                    command_type = 100;
                  }
                }
              }
            }
          }
        }
      }
      int file_size;
      switch (command_type) {
      case 1:
        // 1 改变目录
        CD(param);
        break;
      case 2:
        // 2 创建文件
        file_size = atoi(s1 + 1);
        CREATE(param, 0);
        break;
      case 3:
        // 3 删除文件
        DEL(param);
        break;
      case 4:
        // 4 显示目录
        LSALL();
        break;
      case 5:
        // 5 创建目录
        MD(param);
        break;
      case 6:
        // 6 删除目录
        RD(param);
        break;
      case 0:
        // 0 退出系统
        is_running = 0;
        break;
      default:
        cout << "命令错误" << endl;
      }
    }
  }
}
