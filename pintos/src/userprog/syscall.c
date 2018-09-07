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
#include "filesys/filesys.h"

struct file_descriptor_info
{
	int fd;
	struct file *file_p;
	struct list_elem fd_list_elem;
};

static void syscall_handler (struct intr_frame *);

struct file_descriptor_info* find_fd_p(int fd);

struct lock lock_file_sys;

	void
syscall_init (void) 
{
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");

	// Add lock_init
	lock_init(&lock_file_sys);
}

	static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	void *esp_t;
	int memory_flag = 0;
	esp_t = f->esp;
	if(is_user_vaddr(esp_t) == false)
	{
		memory_flag = 1;
	}
	else
	{
		if(*(int*)esp_t == SYS_HALT)
		{
			func_halt();
		}
		else if(*(int*)esp_t == SYS_EXIT)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				func_exit(*((int*)esp_t + 1));
			}
		}
		else if(*(int*)esp_t == SYS_EXEC)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_exec((const char*)(*((int*)esp_t + 1)));
			}
		}
		else if(*(int*)esp_t == SYS_WAIT)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_wait(*((int*)esp_t + 1));
			}
		}
		else if(*(int*)esp_t == SYS_CREATE)
		{
			if(is_user_vaddr(esp_t + 5) == false)memory_flag = 1;
			else
			{
				f->eax = func_create((const char*)(*((int*)esp_t + 4)),*((unsigned *)esp_t + 5));
			}
		}
		else if(*(int*)esp_t == SYS_REMOVE)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_remove((const char*)(*((int*)esp_t + 1)));
			}
		}
		else if(*(int*)esp_t == SYS_OPEN)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_open((const char*)(*((int*)esp_t + 1)));
			}
		}
		else if(*(int*)esp_t == SYS_FILESIZE)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_filesize(*((int*)esp_t + 1));
			}
		}
		else if(*(int*)esp_t == SYS_READ)
		{
			if(is_user_vaddr(esp_t + 7) == false)memory_flag = 1;
			else
			{
				f->eax = func_read(*((int*)esp_t + 5),(void *)*((int *)esp_t + 6),*((unsigned *)esp_t + 7));
			}
		}
		else if(*(int*)esp_t == SYS_WRITE)
		{
			if(is_user_vaddr(esp_t + 7) == false)memory_flag = 1;
			else
			{
				f->eax=func_write(*((int *)esp_t + 5),(void *)*((int *)esp_t + 6),*((unsigned *)esp_t + 7));
			}
		}
		else if(*(int*)esp_t == SYS_SEEK)
		{
			if(is_user_vaddr(esp_t + 5) == false)memory_flag = 1;
			else
			{
				func_seek(*((int*)esp_t + 4), *((unsigned*)esp_t + 5));
			}
		}
		else if(*(int*)esp_t == SYS_TELL)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = func_tell(*((int*)esp_t + 1));
			}
		}
		else if(*(int*)esp_t == SYS_CLOSE)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				func_close(*((int*)esp_t + 1));
			}
		}
		else if(*(int*)esp_t == SYS_FIBONACCI)
		{
			if(is_user_vaddr(esp_t + 1) == false)memory_flag = 1;
			else
			{
				f->eax = fibonacci(*((int*)esp_t+1));
			}
		}
		else if(*(int*)esp_t == SYS_SUM_INT)
		{
			if(is_user_vaddr(esp_t + 9) == false)memory_flag = 1;
			else
			{
				f->eax = sum_of_four_integers(*((int*)esp_t + 6),*((int*)esp_t + 7),*((int*)esp_t + 8),*((int*)esp_t + 9));
			}
		}
		else
		{
			func_exit(-1);
		}
		if(memory_flag == 1)
		{
			f->eax = -1;
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
	struct thread *cur_t = thread_current();
	struct thread *child_t;
	struct file_descriptor_info *temp_fd_i;
	struct list_elem *e;
	char *token, *save_ptr;
	if(status < 0)status = -1;
	token = strtok_r(cur_t->name," \n",&save_ptr);
	printf("%s: exit(%d)\n",token,status);
	cur_t -> exist_status = status;                 // push status to current thread exist_status

	while(list_empty(&cur_t->fd_list) == false)
	{
		e = list_pop_back(&cur_t->fd_list);
		temp_fd_i = list_entry(e, struct file_descriptor_info, fd_list_elem);
		file_close(temp_fd_i->file_p);
		free(temp_fd_i);
	}
	/*struct thread *p=cur_t->parent_thread;
	while(list_empty(&p->child_list) == false)
	{
		printf("hello\n");
		e = list_pop_back(&cur_t->child_list);
		child_t = list_entry(e, struct thread, child_list_elem);
	}*/
	while(list_empty(&cur_t->child_list) == false)
	{
		e = list_pop_back(&cur_t->child_list);
		child_t = list_entry(e, struct thread, child_list_elem);
	}

	thread_exit();                                  // exit thread
}

// Add function pid_t exec(const char *cmd_line);
int func_exec(const char *cmd_line)
{
	int new_tid;

	//printf("\n\nhi\n\n%s\n",cmd_line);
	new_tid = process_execute(cmd_line);            // make new process

	return new_tid;                                 // return new_tid value

}
// Add function pid_t wait(int pid);
int func_wait(int pid)
{
	return process_wait(pid);
}
// Add function bool create (const char *file, unsigned initial_size);
int func_create (const char *file, unsigned initial_size)
{
	int return_val;

	if(!file)
	{
		func_exit(-1);
	}

	lock_acquire(&lock_file_sys);
	return_val = filesys_create(file,initial_size);
	lock_release(&lock_file_sys);

	return return_val;
}
// Add function bool remove(const char *file);
int func_remove(const char *file)
{
	int return_val;

	if(!file)
	{
		func_exit(-1);
	}

	lock_acquire(&lock_file_sys);
	return_val = filesys_remove(file);
	lock_release(&lock_file_sys);

	return return_val;
}
// Add function int open(const char *file);
int func_open(const char *file)
{
	struct thread *cur_t = thread_current();
	struct file_descriptor_info *temp_fd_i;
	struct file *temp_file;
	unsigned int fd_num;
	int fd_len = 0, i;

	if(file == NULL)
	{
		func_exit(-1);
	}

	lock_acquire(&lock_file_sys);
	temp_file = filesys_open(file);


	if(temp_file != NULL)
	{
		fd_num = cur_t -> fd_num;

		cur_t -> fd_num += 1;

		temp_fd_i = (struct file_descriptor_info *)malloc(sizeof(struct file_descriptor_info));

		temp_fd_i->fd = fd_num;
		temp_fd_i->file_p = temp_file;
		list_push_back(&(cur_t->fd_list),&(temp_fd_i->fd_list_elem));

	}
	else
	{
		fd_num = -1;
	}

	if (strcmp(thread_name(), file) == 0)
	{
		file_deny_write(temp_file);
	}

	lock_release(&lock_file_sys);

	return fd_num;
}
// Add function int func_filesize(int fd);
int func_filesize(int fd)
{
	struct file_descriptor_info *temp_fd_i = NULL;
	struct file *temp_file;
	int return_val;

	temp_fd_i = find_fd_p(fd);

	if(temp_fd_i == NULL)return -1;

	lock_acquire(&lock_file_sys);

	temp_file = temp_fd_i->file_p;
	return_val = file_length(temp_file);

	lock_release(&lock_file_sys);

	return return_val;
}
// Add int read(int fd, void *buffer, unsigned size);
int func_read (int fd, void *buffer, unsigned size)
{
	int read_n = 0;
	unsigned i;
	uint8_t return_val;
	struct file_descriptor_info *temp_fd_i = NULL;


	if(is_user_vaddr(buffer) == false)func_exit(-1);

	lock_acquire(&lock_file_sys);

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
	}
	else
	{
		temp_fd_i = find_fd_p(fd);
		if(!temp_fd_i)read_n = -1;
		else
		{
			read_n = file_read(temp_fd_i->file_p,buffer,size);
		}
	}

	lock_release(&lock_file_sys);

	return read_n;
}

// Add function int write(int fd, const void *buffer, unsigned size);
int func_write(int fd, const void *buffer, unsigned size)
{
	struct file_descriptor_info *temp_fd_i = NULL;
	if(is_user_vaddr(buffer) == false)func_exit(-1);

	lock_acquire(&lock_file_sys);

	if(fd == 1){
		putbuf(buffer, size);
	}
	else
	{
		temp_fd_i = find_fd_p(fd);
		if(temp_fd_i == NULL)size = -1;
		else  
		{
			size = file_write(temp_fd_i->file_p,buffer,size);
		}

	}

	lock_release(&lock_file_sys);

	return size;
}
// Add function void seek(int fd, unsigned position);
void func_seek(int fd, unsigned position)
{
	struct file_descriptor_info *temp_fd_i = NULL;

	temp_fd_i = find_fd_p(fd);

	if(temp_fd_i == NULL)func_exit(-1);
	else
	{	
		lock_acquire(&lock_file_sys);
		file_seek(temp_fd_i->file_p,position);
		lock_release(&lock_file_sys);
	}
}
// Add function unsigned tell (int fd);
unsigned func_tell(int fd)
{
	struct file_descriptor_info *temp_fd_i = NULL;
	unsigned ret;

	temp_fd_i = find_fd_p(fd);

	if(temp_fd_i == NULL)return -1;

	lock_acquire(&lock_file_sys);
	ret=file_tell(temp_fd_i->file_p);
	lock_release(&lock_file_sys);

	return ret;
}
// Add function void close(int fd);
void func_close(int fd)
{
	struct file_descriptor_info *temp_fd_i = NULL;

	temp_fd_i = find_fd_p(fd);

	if(temp_fd_i == NULL)func_exit(-1);
	else
	{
		lock_acquire(&lock_file_sys);

		list_remove(&temp_fd_i->fd_list_elem);

		file_close(temp_fd_i->file_p);

		free(temp_fd_i);

		lock_release(&lock_file_sys);
	}
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

struct file_descriptor_info* find_fd_p(int fd)
{
	struct thread *cur_t = thread_current();
	struct file_descriptor_info *temp_fd_i = NULL;
	struct list_elem *e;
	int find_flag = 0;

	for( e = list_begin(&cur_t->fd_list); e != list_end(&cur_t->fd_list); e = list_next(e))
	{
		temp_fd_i = list_entry(e,struct file_descriptor_info,fd_list_elem);
		if(temp_fd_i->fd == fd)
		{
			find_flag = 1;
			break;
		}
	}

	if(find_flag == 0)return NULL;
	else
	{
		return temp_fd_i;
	}
}
