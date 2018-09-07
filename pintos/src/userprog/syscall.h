#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

// Add header
void func_halt (void);
void func_exit (int status);
int func_exec (const char *cmd_line);
int func_wait(int pid);
int func_create(const char *file, unsigned initial_size);
int func_remove(const char *file);
int func_open(const char *file);
int func_filesize(int fd);
int func_read(int fd, void *buffer, unsigned size);
int func_write (int fd, const void *buffer, unsigned size);
void func_seek(int fd, unsigned position);
unsigned func_tell(int fd);
void func_close(int fd);
int sum_of_four_integers(int a,int b,int c,int d);
int fibonacci(int n);

#endif /* userprog/syscall.h */
