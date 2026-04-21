#include "kernel/types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: getcnt <syscall_number>\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  int count = getcnt(n);

  if(count < 0){
    fprintf(2, "getcnt: invalid syscall number\n");
    exit(1);
  }

  printf("syscall %d has been called %d times\n", n, count);
  exit(0);
}