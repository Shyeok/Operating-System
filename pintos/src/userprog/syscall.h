#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

// Add header
void func_halt (void);
void func_exit (int status);
int func_exec (const char *cmd_line);
int func_wait(int pid);
int func_read(int fd, void *buffer, unsigned size);
int func_write (int fd, const void *buffer, unsigned size);
int sum_of_four_integers(int a,int b,int c,int d);
int fibonacci(int n);

#endif /* userprog/syscall.h */
