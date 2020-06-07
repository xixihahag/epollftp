#ifndef __MY_JUDGE_H__
#define __MY_JUDGE_H__

#include "ftp.h"

static char para[MAXLINE] = {0};

// int servjudge(data *);
void clijudge(data *);
int ana_cmd(char *);
char *get_para(char *, int);

#endif