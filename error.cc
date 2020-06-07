#include "error.h"

int err(char *a){
  printf("%s\n", a);
  return -1;
}

void perr(char *a){
  printf("%s\n", a);
}

void err_sys(char *err){
  printf("%s\n", err);
  exit(1);
}
