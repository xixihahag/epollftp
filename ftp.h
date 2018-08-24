#include "data.h"
#include "my_unp.h"

#define CMD_HELP 00
#define CMD_LS 11
#define CMD_CD 22
#define CMD_PUT 33
#define CMD_GET 44
#define CMD_QUIT 55

#define SENDFILESIZE 1024

int ftp_get_help(int);
int ftp_get_ls(int);
int ftp_get_cd(int, char*);
int ftp_get_put(int ,char*);
int ftp_get_get(int, char*);
int ftp_get_quit(int);

int ftp_put_help(data*);
int ftp_put_ls(data*);
int ftp_put_cd(char*, data*);
int ftp_put_put(char*, data*);
int ftp_put_get(char*, data*);

void show_help();
void init();
