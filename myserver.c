/**
 * [main description]
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 *  问题，需不需要判断对面是否可读
 * 需要判断对面是否可读
 * 需要的话要怎么读和写传数据，不会错乱么
 * 会错乱，通过编程区分开
 * 暂时没考虑一次性传过来的数据会比缓冲区大的问题
 * epoll_event中的data竟然是uniun类型
 *
 * ftp就得考虑一次性数据过大的问题了
 */
#include "my_judge.h"

data mdata[MAXLINE];

int main(int argc, char const *argv[]) {
  int listenfd = network_init(SERVER, NULL);
  struct epoll_event ev, events[EPOLL_SIZE];
  int epfd, connfd;
  int nready, i, j, n, index;
  char *buf;

  for(i=0; i<MAXLINE; i++)
    mdata[i].inuse = FALSE;

  //分出0作为LISTEN
  mdata[0].inuse = TRUE;
  mdata[0].fd = listenfd;

  epfd = epoll_create(EPOLL_SIZE);
  ev.data.u64 = 0;
  ev.events = EPOLLIN;
  epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

  for(;;){
    nready = epoll_wait(epfd, events, EPOLL_SIZE, -1);

    for(i=0; i<nready; i++){
      index = events[i].data.u64;
      if(mdata[index].fd == listenfd){
        //找到一个空的数据结构放进去
        for(j=0; j<MAXLINE; j++)
          if(!mdata[j].inuse)
            break;

        //位置已满，不进行处理
        if(j == MAXLINE)
          continue;

        connfd = Accept(listenfd, NULL, NULL);
        printf("new add in\n");

        //初始化当前工作目录
        strcpy(mdata[j].current_path, "/home/a/Desktop/ftp");
        mdata[j].indeep = 0;
        mdata[j].inuse = TRUE;
        mdata[j].fd = connfd;

        ev.data.u64 = j;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
      }
      else if(events[i].events & EPOLLIN){
        index = events[i].data.u64;
        connfd = mdata[index].fd;
        buf = mdata[index].cmd;
        memset(buf, 0, sizeof(buf));

        if( (n = Read(connfd, buf, CMDSIZE)) > 0){
            mdata[j].size = n;

            //在这处理数据，获得要回传的数据
            //并判断是否有返回值
            if(servjudge(&mdata[j])){
              ev.data.u64 = j;
              ev.events = EPOLLOUT;
              epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
            }
        }
        else if(n == 0){
          epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
          mdata[index].inuse = FALSE;
        }
      }
      else if(events[i].events & EPOLLOUT){
        //这边理论上应该没问题，拿到要回传的数据之后写到客户端就可以了
        index = events[i].data.u64;
        connfd = mdata[index].fd;
        buf = mdata[index].rebuf;
        n = mdata[index].size;

        // printf("myserever rebuf = %s\n", buf);

        Write(connfd, buf, n);

        printf("write success write buf is %s", buf);

        memset(buf, 0, sizeof(buf));

        ev.data.u64 = j;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
      }
    }
  }

  return 0;
}
