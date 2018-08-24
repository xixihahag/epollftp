#include "ftp.h"

char para[MAXLINE];

int servjudge(data*);
void clijudge(data*);
int ana_cmd(char *);
char *get_para(char *, int);
