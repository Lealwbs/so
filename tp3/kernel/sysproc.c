#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_settickets(void)  
{  
  int num_tickets; 
  struct proc *p = myproc();

  acquire(&p->lock);
  argint(0, &num_tickets);

  if(num_tickets < 1){
    release(&p->lock);
    return -1;
  }

  p->tickets = num_tickets;
  release(&p->lock);
  return 0;
}

uint64
sys_getpinfo(void)
{
    struct pstat result;
    uint64 addr;

    argaddr(0, &addr); // pega o ponteiro passado pelo usuário;
    if(addr == 0)
        return -1;

    // A variável 'proc' é o array global de processos no xv6
    extern struct proc proc[NPROC]; 

    for(int i = 0; i < NPROC; i++) {
        acquire(&proc[i].lock); // Protege o acesso a cada processo 
        if(proc[i].state == UNUSED) {
            result.inuse[i] = 0;
            result.pid[i] = 0;
            result.tickets[i] = 0;
            result.ticks[i] = 0;
        } else {
            result.inuse[i] = 1;
            result.pid[i] = proc[i].pid;
            result.tickets[i] = proc[i].tickets;
            result.ticks[i] = proc[i].ticks;
        }
        release(&proc[i].lock);
    }

    if(copyout(myproc()->pagetable, addr, (char *)&result, sizeof(result)) < 0)
        return -1;

    return 0;
}