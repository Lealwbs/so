#include "kernel/types.h"
#include "user.h"
// #define NULL ((void*)0)

int
main(void)
{
  printf("*Antes do acesso*\n");

  int *p = (int*)0;
  printf("valor de p: %p\n", p);

  printf("desreferenciando p...\n");
  int x = *p;   // deve causar trap e matar o processo aqui

  // se chegar aqui, a protecao contra null pointer NAO esta funcionando
  printf("ERRO: nao deveria chegar aqui. valor lido = %d\n", x);

  exit(0);
}