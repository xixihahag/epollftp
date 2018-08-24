#include "my_judge.h"

int ana_cmd(char *cmd){

  // printf("this is ana_cmd\n");

  if( (strcmp(cmd, "help")) == 0){
    //帮助命令，显示所有可用命令
    return CMD_HELP;
  }
  if( (strcmp(cmd, "ls")) == 0){
    //显示服务器上的文件
    return CMD_LS;
  }
  if( (strncmp(cmd, "cd ", 3)) == 0){
    //进入下一个文件夹
    return CMD_CD;
  }
  if( (strncmp(cmd, "get ", 4)) == 0){
    //下载文件
    return CMD_GET;
  }
  if( (strncmp(cmd, "put ", 4)) == 0){
    //上传文件
    return CMD_PUT;
  }
  if( (strcmp(cmd, "quit")) == 0){
    return CMD_QUIT;
  }
}

//返回需要参数的命令的参数
//如 cd ， get ， put
char *get_para(char *buf, int n){
  // return substr(buf, n, strlen(buf)-n+1);
  memset(para, 0, sizeof(para));
  int j=0;
  for(int i=n; i<strlen(buf); i++)
    para[j++] = buf[i];
  para[j] = '\0';

  // printf("buf = %s\n", buf);
  // printf("para = %s\n", para);

  return para;
}
