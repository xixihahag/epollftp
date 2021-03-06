## 基于epoll的ftp服务器

2018-9-7 22:04:09更新

* 将服务器换成非阻塞epoll+et模式
* 修复 cd .. 命令无效的小bug



tips：自己用的时候要将ftp默认路径改成自己的，位置在myserver.c的第61行



接下来的目标

* 添加一个显示当前路径的功能，如果可能的话想做成shell的样子

* 如果没有意外的话看能不能写成epoll线程池的版本

-----



对比上一个版本，这个版本主要就是把服务器换成了epoll版本，并修复了许多小bug

- 接收到的命令，返回数据，工作目录等都是分用户独立出来的
- 修复上传空文件会导致阻塞在read的情况
- 增加cd错误目录报错
- 修复cd目录深度问题
- 重新修订传输数据报格式
- 解决报错信息收不到问题
- 修复多次传输报错信息的错误



然后新的数据包格式如下

- 消息数据报

开头是一个sizeof(int)大小的整形，-1代表后面的数据是报错信息，不是-1代表是正常的信息

接下来也是sizeof(int)大小的整形，代表后面数据的大小

再后面接的是真实的数据



- 传输文件数据报

开头是一个sizeof(int)大小的整形，代表后面数据 的大小，并且根据这个大小和buf缓冲区的大小作比较来判断是否传输完毕

后面接的是真实的数据



- 每个用户自己的数据结构如下

  ```c
  typedef struct Data{
    int fd;						//sockfd
    char cmd[CMDSIZE];   			 //用户输入的命令
    char rebuf[MAXLINE];			 //保存服务器回传的数据，一般是错误信息
    int size;						//rebuf的大小
    int inuse;					//此data是否在被使用
    char current_path[MAXLINE];	  //保存当前用户工作目录
    int indeep;					 //报错工作目录深度，避免无限向上cd
  }data;
  ```

  

- 各文件依赖图

  ![IMG_20180824_093728](http://pdsoe2moe.bkt.clouddn.com/IMG_20180824_093728.jpg)

手画的，画的有点烂，但是能看清哈。

其中 a--->b 箭头表示a include b这个文件。



- 完善目标

  加入登录功能。完善用户各种状态信息。服务器端客户端往返数据规范化。

  

- 命令说明

  - 编译，已经写好了makefile，直接make就行

  - ls用法

    ```c
    ls
    ```

    获取到服务器端的文件目录

  - help用法

    ```c
    help
    ```

    获取到服务器端支持命令列表

  - cd命令

    ```c
    cd ..
    cd .
    cd xxx
    ```

    和linux上用法一样

  - get

    ```c
    get filename
    ```

    获取到当前目录下的filename文件，默认存储在./recvftp文件夹内

  - put

    ```c
    put 绝对路径
    ```

    上传文件到服务器，注意这里是绝对路径

  - quit

    ```c
    quit
    ```

    单纯的退出指令，只退出客户端。



* 食用方法

```c++
mkdir build && cd build
cmake .. && make
./server
./client server_ip
```

