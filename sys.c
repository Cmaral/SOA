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

  // creates the child process
  
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




