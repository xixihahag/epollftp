#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <poll.h>
#include <errno.h>
#include <sys/stropts.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERV_PORT 9877
#define SA struct sockaddr
#define LISTENQ 10240
#define MAXLINE 10240
#define SERVER 2
#define CLIENT 3
#define OPEN_MAX 10240
#define INFTIM -1
#define EPOLL_SIZE 10240
#define TRUE 1
#define FALSE 0
#define CMDSIZE 1024

int network_init(int, char *, int);
size_t Read(int, void*, size_t);
void Write(int, void*, size_t);
int Accept(int, SA*, socklen_t*);
int setnonblocking(int);
