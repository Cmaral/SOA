/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

extern zeos_ticks;
extern struct list_head freequeue;

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;
  
  if (list_empty(&freequeue)) return -1;
  else {
        struct list_head *fork_task_head;
        fork_task_head = list_first(&freequeue);
        list_del(fork_task_head);
        
        struct task_struct *fork_task_struct;
        fork_task_struct = list_head_to_task_struct(fork_task_head);
        
        union task_union *fork_task_union;
        fork_task_union = (union task_union *)fork_task_struct;
        
        union task_union *padre_task_union;
        padre_task_union = (union task_union *) current();
        
        copy_data(&padre_task_union->stack[KERNEL_STACK_SIZE-1024], &fork_task_union->stack[KERNEL_STACK_SIZE-1024], 4096);
        
        allocate_DIR(fork_task_struct);
        
        
        
  }
  
  return PID;
}

void sys_exit()
{  
}

int sys_gettime() {
       return zeos_ticks;
}

int sys_write(int fd, char* buffer, int size) {
    int bytes_escrits = 0;
    char buffer_sys[4096];
    int error = check_fd(fd, ESCRIPTURA);
    if (error != 0) return error;
    if (size < 0) return -1;
    if (buffer == NULL) return -105;
    while (size >= 4096) {
        copy_from_user(buffer, buffer_sys, 4096);
        buffer += 4096;
        bytes_escrits += sys_write_console(buffer_sys, 4096);
        size -= 4096;
    }
    char buffer_sys_b[size];
    copy_from_user(buffer, buffer_sys_b, size);
    bytes_escrits += sys_write_console(buffer_sys_b, size);
    return bytes_escrits;
}




