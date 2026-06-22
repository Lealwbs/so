# TP3: xv6 Null-Pointer Dereference Protection

## Resources
- Resources for assignment 1 and assignment 2 remain useful.
- **Recommended reading:** [Xv6 book](D:\Code\tp3-sistemas-operacionais\docs\book-riscv-rev3.pdf), Sections 3.6–3.9.
- **Assignment source/inspiration:** [OSTEP Intro to xv6 Virtual Memory](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/vm-xv6-intro)

---

## Tasks
We will implement the **null-pointer dereference** improvements to xv6, as described in the OSTEP assignment.

> **Read-only code regions (the second half of the OSTEP project) are NOT part of this assignment.**
> You only need to ensure your program correctly handles invalid memory accesses, as described at the bottom of the OSTEP assignment description.

When implementing your changes, you should make sure that:
- Processes cannot ever dereference a null pointer.
- Processes that dereference a null pointer are correctly flagged by the kernel as making an invalid memory access (and killed accordingly).
- The kernel does not leak memory (i.e., all memory used by a process must be freed upon termination).
- There is no significant memory or CPU overhead for protecting against null-pointer dereferences.

---

## What to Submit
You should upload a `.zip` containing the complete version of your modified xv6 with your null-pointer dereference changes. You should also upload documentation in **PDF format** with three sections:

### 1. xv6 Modifications
Discuss the modifications you made to the xv6 code. When appropriate, use the `git diff` command to compute the changes between the original xv6 code and your modified version.
Explain the following:
- What is your strategy for making the first page invalid? Where did you move user programs?
- What changes were necessary in the kernel to relocate programs?
- What changes were necessary in the build process (Makefile / linker configuration) to relocate programs?
- What changes were necessary to handle illegal references to the null address?

### 2. Process Address Space Description
Create a diagram/image of the xv6 program address space including your null-pointer dereference changes. Use **Figure 3.4** (the address space description in the chapter about the `exec` syscall in the xv6 book) as a baseline.

### 3. Tests
Discuss how you tested that your changes work correctly.

---

## Project Context (From OSTEP)

### Intro to xv6 Virtual Memory
In this project, you'll be changing xv6 to support a feature virtually every modern OS does: causing an exception to occur when your program dereferences a null pointer.

### Null-pointer Dereference Details
In xv6, the VM system uses a simple page table. As it currently is structured, user code is loaded into the very first part of the address space (starting at virtual address `0`). Thus, if you dereference a null pointer, you will not see an exception; rather, you will see whatever code/data is at the beginning of the program.

To see the difference, you can create a simple user program that dereferences a null pointer and run it on both Linux and the unmodified xv6.

Your job here will be to figure out how xv6 sets up a page table. This project is mostly about understanding the code, and not writing very much:
- Look at how `exec()` works to better understand how address spaces get filled with code and initialized.
- Look at `fork()`, in particular the part where the address space of the child is created by copying the address space of the parent. What needs to change in there?
- Look through the code to figure out where there are checks or assumptions made about the address space. Think about what happens when you pass a parameter into the kernel, for example; if passing a pointer, the kernel needs to be very careful with it to ensure you haven't passed a bad pointer (e.g., check `copyin()`, `copyout()`, and argument parsing functions like `argaddr()`). How does it do this now? Does this code need to change in order to work in your new version of xv6?
- **Makefile/Linker changes:** Look at the xv6 Makefile. User programs are compiled so as to set their entry point (where the first instruction is) to `0`. If you change xv6 to make the first page invalid, clearly the entry point will have to be somewhere else (e.g., the next page, or `0x1000`). Thus, something in the Makefile/compilation process will need to change to reflect this.

---

> ### Read-only Code (Out of Scope for this TP)
> *The following section is kept for context from the original OSTEP project but is **not** required for this assignment.*
>
> In most operating systems, code is marked read-only instead of read-write. In this portion of the OSTEP project, you'd change the protection bits of parts of the page table to be read-only using two new system calls:
> - `int mprotect(void *addr, int len)`
> - `int munprotect(void *addr, int len)`
>
> **Handling Illegal Accesses:**
> In both cases, user code trying to access a null pointer or overwrite an mprotected region of memory should cause xv6 to trap and kill the process.