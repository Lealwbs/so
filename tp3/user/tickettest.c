#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define TEST_START(name) printf("TEST %s: STARTING...\n", name)
#define TEST_END() printf("# TEST FINISHED\n\n")

void test_1(void) {
  TEST_START("#1 - getpinfo default test");
  
  struct pstat result; 
  int id = getpinfo(&result);
  
  // Relatório completo do getpinfo
  printf("ID: %d\n", id);
  if(id!=0) return;
  printf("Iter \t| PID \t| Tickt\t| Ticks\t| InUse \t \t\n");
  printf("----------------------------------------------------------\n");
  for(int i = 0; i < NPROC; i++){
      printf("%d \t| %d  \t| %d  \t| %d  \t| %d\n", 
            i, result.pid[i], result.tickets[i], result.ticks[i], result.inuse[i]);
  }
  TEST_END();
}

void test_2(void) {
  TEST_START("#2 - tickets lottery scheduling");

  int const PROCESSES = 6;
  int const WORKSIZE = 1e9;
  int tickets[] = {1, 4, 15, 64, 256, 660};
  int pids[PROCESSES];

  // Criação dos processos
  for(int i = 0; i < PROCESSES; i++){
    pids[i] = fork();
    if(pids[i] == 0) { 
      settickets(tickets[i]);
      volatile int work = 0;
      for(int j = 0; j < WORKSIZE; j++) work++; 
      exit(0);
    }
  }

  // Aguarda pela finalização de todos os processos.
  struct pstat info;
  int completed = 0;
  while(completed < PROCESSES) {
    int start = uptime();
    while(uptime() < start + 50) {}
    if(getpinfo(&info) != 0) continue;

    // Verifica se algum dos nossos PIDs ainda está 'inuse'
    completed = 0;
    for(int i = 0; i < NPROC; i++) {
      for(int k = 0; k < PROCESSES; k++) {
        if(info.inuse[i] && info.pid[i] == pids[k]) completed++;
      }
    }
  }
  
  // Imprime o relatório dos processos 'inuse'
  struct pstat result; 
  if(getpinfo(&result)!=0) return;
  printf("Iter \t| PID \t| Tickt\t| Ticks\t| InUse \t \t\n");
  printf("----------------------------------------------------------\n");
  for(int i = 0; i < NPROC; i++){
      if(result.inuse[i]) 
        printf("%d \t| %d  \t| %d  \t| %d  \t| %d\n", i, result.pid[i], 
          result.tickets[i], result.ticks[i], result.inuse[i]);
  }

  for(int i = 0; i < 3; i++) wait(0);
  TEST_END();
}

int main(int argc, char **argv) {
  // Se o usuário passar um argumento, rode apenas um teste específico
  if (argc > 1) {
    if (strcmp(argv[1], "1") == 0) test_1();
    if (strcmp(argv[1], "2") == 0) test_2();
  } else {
    test_1(); 
    test_2(); 
    printf("ALL TESTS PASSED!\n");
  }
  exit(0);
}