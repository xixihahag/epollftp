#include "my_judge.h"
#include "error.h"

int n;
char buf[MAXLINE];
char recv_path[MAXLINE] = "./recvftp";
char msg[MAXLINE];

void init(){
  memset(buf, 0, sizeof(buf));
}

//--------------------------------------------------
//客户端请求函数
int ftp_get_help(int sockfd){
  init();
  write(sockfd, "help", sizeof("help"));
  if( (n = read(sockfd, buf, MAXLINE)) > 0)
    write(fileno(stdout), buf, sizeof(buf));
  if(n < 0)
    return err("ftp_get_help error\n");
}


int ftp_get_ls(int sockfd){

  // printf("this is ftp_get_ls\n");

  init();
  write(sockfd, "ls", sizeof("ls"));

  // printf("ready for read\n");
  if( (n = read(sockfd, buf, MAXLINE)) >= 0){
    // printf("read success, ready for write\n");
    write(fileno(stdout), buf, sizeof(buf));
    // printf("write success\n");
  }
  else if(n < 0)
    return err("ftp_get_ls error\n");

  // printf("read finished\n");
  // printf("return ftp_get_ls\n");
}


int ftp_get_cd(int sockfd, char *cmd){
  //strlen 写成 sizeof 导致传输过去少一个字节，查了好半天
  init();
  int flag;
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_cd error\n");
  else{
    read(sockfd, buf, MAXLINE);
    memcpy(&flag, buf, sizeof(int));
    if(flag == 1)
      return 1;
    // printf("buf = %s\n", buf);
    memcpy(msg, buf+sizeof(int), strlen(buf)-sizeof(int));
    printf("%s\n", msg);
  }
  return 1;
}

//上传文件
int ftp_get_put(int sockfd, char *cmd){
  init();
  char send[SENDFILESIZE];
  int filefd;
  int sendsize;
  if( (filefd = open(get_para(cmd, 4), O_RDONLY)) == -1)
    return err("ftp_get_put open error\n");

  // printf("open file success\n");

  //告诉服务器准备接受数据
  if( (write(sockfd, cmd, strlen(cmd))) < 0)
    return err("ftp_get_put write error\n");

  // printf("ready to send file\n");

  //准备传输数据
  while( (sendsize = read(filefd, (send+sizeof(int)), (SENDFILESIZE-sizeof(int)))) > 0){

    // printf("sendsize = %d\n", sendsize);

    memcpy(send, &sendsize, sizeof(int));
    if( (write(sockfd, send, SENDFILESIZE)) < 0){
      close(filefd);
      return err("put file error\n");
    }
    memset(send, 0, sizeof(send));
  }
  close(filefd);
  printf("upload file success\n");
  return 0;
}


int ftp_get_get(int sockfd, char *cmd){
  init();
  char *filename = get_para(cmd, 4);
  char filepath[MAXLINE];
  int filesize;
  int filefd, flag;

  //当前目录下还没有创建recv_path这个文件夹
  if(opendir(recv_path) == NULL)
    mkdir("recvftp", 0777);

  strcat(filepath, recv_path);
  strcat(filepath, "/");
  strcat(filepath, filename);

  // printf("filepath = %s\n", filepath);

  if(write(sockfd, cmd, strlen(cmd)) < 0)
    return err("ftp_get_get write error\n");
  else{
    if(read(sockfd, buf, MAXLINE) >= 0){
      //收到数据先判断是不是报错信息
      memcpy(&flag, buf, sizeof(int));

      // printf("buf = %s\n", buf);
      // printf("flag = %d\n", flag);
      if(flag == -1){
        //返回的是出错信息
        printf("%s\n", buf+sizeof(int));
        return -1;
      }
      else{
        // printf("filesize = %d\n", filesize);
        // memcpy(&filesize, buf+sizeof(int), sizeof(int));
        if( (filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0){
          return err("ftp_get_get open file error\n");
        }

        //循环接收文件
        // printf("ready for receive file\n");

        int con = 1;
        while(con){

          // printf("in while\n");

          memcpy(&filesize, buf+sizeof(int), sizeof(int));

          // printf("filesize = %d\n", filesize);

          if( (write(filefd, buf+2*sizeof(int), filesize)) < 0){
            close(filefd);
            return err("ftp_get_get write to file error\n");
          }

          if(filesize < (SENDFILESIZE-2*sizeof(int))){
            con = 0;
            // printf("con = 0\n");
          }
          else
            read(sockfd, buf, SENDFILESIZE);

        }
      }
    }
  }
  printf("file received success\n");
  return 0;
}


// int ftp_get_quit(int sockfd){
//   write(sockfd, "quit", sizeof("quit"));
//   return 0;
// }
