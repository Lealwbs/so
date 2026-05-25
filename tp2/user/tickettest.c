#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define TEST_START(name) printf("TEST %s: STARTING...\n", name)
#define TEST_END() printf("# TEST FINISHED\n\n")

void test_lottery(int tickets[], int const PROCESSES, long const WORKSIZE) {
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
  for(int i = 3; i < NPROC; i++){
      if(result.inuse[i]) 
        printf("%d \t| %d  \t| %d  \t| %d  \t| %d\n", i, result.pid[i], 
          result.tickets[i], result.ticks[i], result.inuse[i]);
  }

  for(int i = 0; i < PROCESSES; i++) wait(0);
}

void test_1(void) {
  TEST_START("#1 - getpinfo default report");
  struct pstat result; 
  int id = getpinfo(&result);
  printf("ID: %d\nIter \t| PID \t| Tickt\t| Ticks\t| InUse \t \t\n", id);
  printf("----------------------------------------------------------\n");
  for(int i = 0; i < NPROC; i++) printf("%d \t| %d  \t| %d  \t| %d  \t| %d\n", 
    i, result.pid[i], result.tickets[i], result.ticks[i], result.inuse[i]);  
  TEST_END();
}

void test_2(void){
  TEST_START("#2 - lottery sparse tickets");
  int tickets[] = {1, 4, 15, 64, 256, 660};
  test_lottery(tickets, 6, 1e11);
  TEST_END();
}

void test_3(void){
  TEST_START("#3 - lottery few tickets");
  int tickets[] = {10, 20, 30, 40};
  test_lottery(tickets, 4, 1e11);
  TEST_END();
}

void test_4(void){
  TEST_START("#4 - lottery small workload");
  int tickets[] = {1, 4, 16, 64, 256};
  test_lottery(tickets, 5, 1e9);
  TEST_END();
}

void test_5(void){
  TEST_START("#5 - lottery equal tickets");
  int tickets[] = {1, 1, 1, 3, 3, 3, 9, 9, 9};
  test_lottery(tickets, 9, 1e11);
  TEST_END();
}

int main(int argc, char **argv) {
  // Se o usuário passar um argumento, rode apenas um teste específico
  if (argc > 1) {
    if (strcmp(argv[1], "1") == 0) test_1();
    if (strcmp(argv[1], "2") == 0) test_2();
    if (strcmp(argv[1], "3") == 0) test_3();
    if (strcmp(argv[1], "4") == 0) test_4();
    if (strcmp(argv[1], "5") == 0) test_5();
  } else {
    test_1(); 
    test_2(); 
    test_3(); 
    test_4(); 
    test_5(); 
    printf("ALL TESTS PASSED!\n");
  }
  exit(0);
}