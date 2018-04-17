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

#include <stats.h>

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

int ret_from_fork(){
  return 0;
}


int sys_fork()
{
  // Get new free task_struct, error if there is no space
  if (list_empty(&freequeue)) return -1;

    // If task_struct available, continue
  struct list_head *fork_list_head;
  fork_list_head = list_first(&freequeue);
  list_del(fork_list_head);

  // Copy parent task_union to child
  union task_union *parent_task_union;
  parent_task_union = (union task_union *) current();

  union task_union *fork_task_union;
  struct task_struct *fork_task_struct;
  fork_task_struct = list_head_to_task_struct(fork_list_head);

  fork_task_union = (union task_union *)fork_task_struct;

  copy_data(parent_task_union, fork_task_union, sizeof(union task_union)); // Copy parent->child

  // Initialize new directory

  allocate_DIR(fork_task_struct);

  // Map logical pages

  int fork_frames[NUM_PAG_DATA]; // Data pages per task
  /* Allocate frames
  Check if there are enough available frames 
  Free allocated frames if there aren't enough */

 
  for (int i=0; i<NUM_PAG_DATA; i++) {
    fork_frames[i] = alloc_frame();
    if (fork_frames[i] == -1) {
      // Error, not enough frames
      // Free allocated frames
      for (int j=0; j<i; j++) free_frame(fork_frames[j]);
      return -1;
    }
  }

  // Inherit user data

    /* Obtain page tables */
    page_table_entry *fork_page_table;
    page_table_entry *parent_page_table;

    fork_page_table = get_PT(fork_task_struct);
    parent_page_table = get_PT(&parent_task_union->task);

    /* Fill child page table with parent info */

    // Copy system code and data
    // Use same frames as parent
    for (int i=0; i<PAG_LOG_INIT_DATA; i++) {
      set_ss_pag(fork_page_table, i, get_frame(parent_page_table, i));
    }

    // Find temporal free entry in parent's table

    int temporal_page = -1;
    int first_page = PAG_LOG_INIT_DATA+NUM_PAG_DATA;
    for (int i=first_page; i<TOTAL_PAGES; i++) {
      if (parent_page_table[i].entry == 0) {
        temporal_page = i; // Free entry found
        break; // Exit
      }
    }

    if (temporal_page == -1) return -1; // Error, no free page table entry found

    void *parent_page_address;
    void *fork_page_address;

    // New pages for user data+stack, and copy
    for (int i=0; i<NUM_PAG_DATA; i++) {
      set_ss_pag(fork_page_table, PAG_LOG_INIT_DATA+i, fork_frames[i]); // Set page->frame
      set_ss_pag(parent_page_table, temporal_page, fork_frames[i]); // Set temporal page->frame (same frame)
      /* Now both are associated with the same frame, we can copy */

      //Copy parent data to child     
      parent_page_address = (void *)((PAG_LOG_INIT_DATA+i)*PAGE_SIZE);
      fork_page_address = (void *)((temporal_page)*PAGE_SIZE);

      copy_data(parent_page_address, fork_page_address, PAGE_SIZE);
      // Free temporal entry + flush TLB, parent shouldn't access child pages
      del_ss_pag(parent_page_table, temporal_page); 
      set_cr3(get_DIR(&parent_task_union->task));
    }

  // Assign new PID
  fork_task_struct->PID = get_new_pid();
  parent_page_table[temporal_page].entry = 0; // Free page

  // Initialize non-common fields, prepare for task_switch
  fork_task_struct->kernel_esp = &fork_task_union->stack[KERNEL_STACK_SIZE-19];
  fork_task_union->stack[KERNEL_STACK_SIZE-19] = 0;
  fork_task_union->stack[KERNEL_STACK_SIZE-18] = &ret_from_fork;

  // Add to readyqueue
  list_add_tail(&fork_task_struct->list, &readyqueue);
  fork_task_struct->state = ST_READY;
  fork_task_struct->quantum = 100;

  // Return PID
  return fork_task_struct->PID;
 
}

void sys_exit()
{  
    int task_frames[NUM_PAG_DATA];
    for (int i=0; i<NUM_PAG_DATA; i++) {
      free_frame(task_frames[i]);
    }
    list_del(&(current()->list));
    list_add_tail(&(current()->list), &freequeue);
    current()->PID = -1;
    schedule();

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

int sys_get_stats(int pid, struct stats *st){
	
  // Si el proceso actual ya es el que queremos, cogemos stats directamente
	if(current()->PID == pid) {
		copy_to_user(&(current()->st), st, sizeof(struct stats));
		return 0;
	}	else {
    // Miramos PID de los demás procesos hasta encontrar el que queremos
		for(int i=0; i<NR_TASKS; i++){
			if(task[i].task.PID == pid){
        // Copiamos stats del proceso encontrado
				copy_to_user(&(task[i].task.st), st, sizeof(struct stats));
				return 0;
			}
		}
	
	}
	return -1;
}



