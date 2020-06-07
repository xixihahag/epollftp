#include "my_judge.h"

int main(int argc, char **argv)
{
  int sockfd;
  char buf[MAXLINE];
  int cmd, end = 0;

  if (argc < 2)
  {
    printf("please input server ip\n");
    return 0;
  }

  sockfd = network_init(CLIENT, argv[1], FALSE);

  for (;;)
  {
    if (end == 1)
      break;
    printf("please input cmd\n");
    memset(buf, 0, sizeof(buf));
    read(fileno(stdin), buf, MAXLINE);
    buf[strlen(buf) - 1] = '\0';
    cmd = ana_cmd(buf);
    switch (cmd)
    {
    case CMD_HELP:
      ftp_get_help(sockfd);
      break;
    case CMD_LS:
      ftp_get_ls(sockfd);
      break;
    case CMD_CD:
      ftp_get_cd(sockfd, buf);
      break;
    case CMD_PUT:
      ftp_get_put(sockfd, buf);
      break;
    case CMD_GET:
      ftp_get_get(sockfd, buf);
      break;
    case CMD_QUIT:
      close(sockfd);
      end = 1;
      break;
    default:
      printf("cmd error please input again\n");
    }
  }

  return 0;
}
