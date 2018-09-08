#include "my_judge.h"

int servjudge(data *mdata){

  // printf("this is servjudge\n");

  int result = ana_cmd(mdata->cmd);
  int connfd = mdata->fd;
  switch(result){
    case CMD_HELP:
      ftp_put_help(mdata);
      break;
    case CMD_LS:
      printf("receive ls command\n");
      ftp_put_ls(mdata);
      break;
    case CMD_CD:
      printf("receive cd command\n");
      return ftp_put_cd(get_para(mdata->cmd, 3), mdata);
      break;
    case CMD_PUT:
      ftp_put_put(get_para(mdata->cmd, 4), mdata);
      return 0;
      break;
    case CMD_GET:
      // printf("receive get command\n");
      return ftp_put_get(get_para(mdata->cmd, 4), mdata);
      break;
  }
  return 1;
}
