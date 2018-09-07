#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include <string.h>
// Add header
#include "devices/shutdown.h"
#include "lib/kernel/console.h"
#include "devices/input.h"
#include "threads/vaddr.h"
#include "process.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	void *esp_t;
	esp_t = f->esp;
	if(is_user_vaddr(esp_t) == false)
	{
		f->eax=-1;
		func_exit(-1);
	}
	else
	{
		if(*(int*)esp_t == SYS_HALT)
		{
			func_halt();
		}
		else if(*(int*)esp_t == SYS_EXIT)
		{
			func_exit(*((int*)esp_t + 1));
		}
		else if(*(int*)esp_t == SYS_EXEC)
		{
			f->eax = func_exec((const char*)(*((int*)esp_t + 1)));
		}
		else if(*(int*)esp_t == SYS_WAIT)
		{
			f->eax = func_wait(*((int*)esp_t + 1));
		}
		else if(*(int*)esp_t == SYS_READ)
		{
			f->eax = func_read(*((int*)esp_t + 5),(void *)*((int *)esp_t + 6),*((unsigned *)esp_t + 7));
		}
		else if(*(int*)esp_t == SYS_WRITE)
		{
			f->eax = func_write(*((int *)esp_t + 5),(void *)*((int *)esp_t + 6),*((unsigned *)esp_t + 7));
		}
		else if(*(int*)esp_t == SYS_FIBONACCI)
		{
			f->eax = fibonacci(*((int*)esp_t+1));
		}
		else if(*(int*)esp_t == SYS_SUM_INT)
		{
			f->eax = sum_of_four_integers(*((int*)esp_t + 6),*((int*)esp_t + 7),*((int*)esp_t + 8),*((int*)esp_t + 9));
		}
		else
		{
			func_exit(-1);
		}

	}
}
// Add function void halt();
void func_halt()
{
	shutdown_power_off();      // calling shutdown_power_off() for terminates pintos
}

// Add function void exit(int status);
void func_exit(int status)
{
	enum intr_level old_level;
	struct thread *cur_t = thread_current();
	struct thread *s;
	char *token, *save_ptr;
	if(status < 0)status = -1;
	token = strtok_r(cur_t->name," \n",&save_ptr);
	printf("%s: exit(%d)\n",token,status);

	cur_t -> exist_status = status;                 // push status to current thread exist_status


	if(cur_t->parent_thread != NULL)
	{
		cur_t->exit_flag = true;
	}
	thread_exit();                                  // exit thread
}

// Add function pid_t exec(const char *cmd_line);
int func_exec(const char *cmd_line)
{
	int new_tid;

	new_tid = process_execute(cmd_line);            // make new process

	return new_tid;                                 // return new_tid value

}
// Add function pid_t wait(int pid);
int func_wait(int pid)
{
	return process_wait(pid);
}
// Add int read(int fd, void *buffer, unsigned size);
int func_read (int fd, void *buffer, unsigned size)
{
	int read_n;
	unsigned i;
	uint8_t return_val;
	if(fd == 0)
	{
		read_n = 0;
		for(i=0; i < size; i++)
		{
			return_val = input_getc();
			*(char*)buffer = return_val;
			buffer += 1;
			read_n += 1;
			if(read_n == (uint8_t)size)break;
		}
		return read_n;
	}
	return 0;
}

// Add function int write(int fd, const void *buffer, unsigned size);
int func_write(int fd, const void *buffer, unsigned size)
{
	if(fd == 1)
	{
		putbuf(buffer, size);
	}
	return size;
}

// Add function function int fibonacci(int n);
int fibonacci(int n)
{
	int n_0, n_1, n_t;
	n_0 = 1;
	n_1 = 1;
	if(n == 1)return n_0;
	else if(n == 2)return n_1;
	else if(n > 2)
	{
		n -= 2;
		while(n)
		{
			n_t = n_1;
			n_1 = n_1 + n_0;
			n_0 = n_t;
			n -= 1;
		}
		return n_1;
	}
	return -1;
}

// Add function sum_of_four_integer(char *str_0, char *str_1, char *str_2, char *str_3);
int sum_of_four_integers(int a,int b,int c,int d)
{

	return a+b+c+d;
}

