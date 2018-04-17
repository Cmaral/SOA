/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */
struct task_struct *idle_task;
union task_union *init_union;


struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}


extern struct list_head blocked;
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;
int new_pid;
int global_quantum=QUANTUM;
void task_switch(union task_union * new);


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");
        
	while(1)
	{
	 ;
	}
}



/*Initializes idle process */
void init_idle (void)
{
  
    struct list_head *idle_task_head;
    idle_task_head = list_first(&freequeue); //Agafem el primer disponible
    list_del(idle_task_head); // Eliminem de la freequeue
    
    struct task_struct *idle_task_struct;
    idle_task_struct = list_head_to_task_struct(idle_task_head); //Agafem task_struct
    
    union task_union *idle_task_union;
    idle_task_union = (union task_union *)idle_task_struct; //Type cast a task_union
    
    
    idle_task_struct->PID = 0; //Assignem PID 0
    allocate_DIR(idle_task_struct); //Alloquem nou directori
    
    idle_task = idle_task_struct; //Inicialitzem idle_task

//Per fer el context switch:
    idle_task_union->stack[KERNEL_STACK_SIZE-1]=&cpu_idle; //Apunta a cpu_idle
    idle_task_union->stack[KERNEL_STACK_SIZE-2]=0; //
    idle_task_union->task.kernel_esp = &idle_task_union->stack[KERNEL_STACK_SIZE-2]; //Per retornar
    
    idle_task->st.user_ticks = 0;
    idle_task->st.system_ticks = 0;
    idle_task->st.blocked_ticks = 0;
    idle_task->st.ready_ticks = 0;
    idle_task->st.elapsed_total_ticks = 0;
    idle_task->st.total_trans = 0;
    idle_task->st.remaining_ticks = 0;
    idle_task->quantum = QUANTUM;

}


void init_task1(void)
{
    struct list_head *init_task_head;
    init_task_head = list_first(&freequeue);
    list_del(init_task_head);
    
    struct task_struct *init_task_struct;
    init_task_struct = list_head_to_task_struct(init_task_head);
    
    union task_union *init_task_union;
    init_task_union = (union task_union *)init_task_struct;
    
    init_task_struct->PID = 1;
    init_task_struct->state = ST_RUN;
    init_task_struct->quantum = QUANTUM;
    new_pid = 10;
    
    init_task_struct->st.user_ticks = 0;
    init_task_struct->st.system_ticks = 0;
    init_task_struct->st.blocked_ticks = 0;
    init_task_struct->st.ready_ticks = 0;
    init_task_struct->st.elapsed_total_ticks = 0;
    init_task_struct->st.total_trans = 0;
    init_task_struct->st.remaining_ticks = 0;


    init_union = init_task_union;

    allocate_DIR(init_task_struct);
    set_user_pages(init_task_struct);
    
    tss.esp0 = &init_task_union->stack[KERNEL_STACK_SIZE];
    set_cr3((init_task_struct->dir_pages_baseAddr));
}

void init_sched(){
    //Inicialitza freequeue
    INIT_LIST_HEAD(&freequeue);
    int i;
    for (i=0; i<NR_TASKS;i++){
      list_add(&(task[i].task.list), &freequeue); //Inicialitza freequeue amb tasks
    }

    //Inicialitza readyqueue
    INIT_LIST_HEAD(&readyqueue);
}


struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}


void inner_task_switch(union task_union * new) {
  tss.esp0 = &(new->stack[KERNEL_STACK_SIZE]);
  set_cr3(new->task.dir_pages_baseAddr);
  assem_inner_task_switch(new->task.kernel_esp, &(current()->kernel_esp));
}

int get_new_pid() {
  new_pid += 1;
  return new_pid;
}

int get_quantum (struct task_struct* t) {
    return t->quantum;
}

void set_quantum (struct task_struct* t, int new_quantum) {
    t->quantum = new_quantum;
}

void update_sched_data_rr () {
    --global_quantum;
}

int needs_sched_rr() {
    if ((global_quantum <= 0)&&(!list_empty(&readyqueue))) return 1;
    if (global_quantum <= 0) global_quantum = get_quantum(current());
    return 0;
}

void update_process_state_rr (struct task_struct* t, struct list_head* dst_queue) {
    if (dst_queue == NULL) {
               
        list_del(&t->list);        
        t->state = ST_RUN;
        
    }
    else {
        
        t->state = ST_READY;
        list_add_tail(&t->list, dst_queue);
    }
}

void sched_next_rr() {
    struct list_head *next_task_head;
    struct task_struct *next_task_struct;

    if (!list_empty(&readyqueue)) {
        next_task_head = list_first(&readyqueue);
        list_del(next_task_head);
        next_task_struct = list_head_to_task_struct(next_task_head);
    } else {
        next_task_struct=idle_task;
        next_task_struct->state = ST_RUN;
        global_quantum = get_quantum(next_task_struct);
    
    union task_union *next_task_union;
    next_task_union = (union task_union *)next_task_struct;
    
    update_process_state_rr(&next_task_struct, NULL);
    task_switch(next_task_union);
    }
}

void schedule() {

    if (current()->PID == 0) {
        if (!list_empty(&readyqueue)) {
            sched_next_rr();
        }
        else return;
    }
    else {
        if (needs_sched_rr()) {
            printk("needs sched");
            if (list_empty(&readyqueue)) {
                union task_union *idle_task_union;
                 idle_task_union = (union task_union *)idle_task; //Type cast a task_union
                 task_switch(idle_task_union);
            }
            else {                
                update_process_state_rr(current(), &readyqueue);
                sched_next_rr();
                set_quantum(current(), QUANTUM);
            }
        }
        else {
            update_sched_data_rr();
        }
    }
}

// usuari -> system
void update_stats_user_system(){
    current()->st.user_ticks += get_ticks() - current()->st.elapsed_total_ticks;
    current()->st.elapsed_total_ticks = get_ticks();
}

// system -> usuari
void update_stats_system_user(){
    current()->st.system_ticks += get_ticks() - current()->st.elapsed_total_ticks;
    current()->st.elapsed_total_ticks = get_ticks();
}

// system -> ready
// aquesta es igual que l'anterior, la podriem esborrar i fer servir l'altre
void update_stats_system_ready(){
    current()->st.system_ticks += get_ticks() - current()->st.elapsed_total_ticks;
    current()->st.elapsed_total_ticks = get_ticks();
}

// ready -> system
void update_stats_ready_system(){
    current()->st.ready_ticks += get_ticks() - current()->st.elapsed_total_ticks;
    current()->st.elapsed_total_ticks = get_ticks();
}
