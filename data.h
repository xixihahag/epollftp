#define CMDSIZE 1024
#define MAXLINE 10240

typedef struct Data{
  //是否要做成循环结构，那种非阻塞类型的
  int fd;
  char cmd[CMDSIZE];
  char rebuf[MAXLINE];
  int size;
  int inuse;
  char current_path[MAXLINE];
  int indeep;
}data;
