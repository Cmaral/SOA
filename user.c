#include <libc.h>


char buff[24];

//Mostra per pantalla els stats del process amb pid task_pid
void stats(int task_pid) {
	int total_time;
	struct stats task_stats;


	itoa(task_pid,buff);
	write(1, "Proc1 ", 7);
	write(1, buff, strlen(buff));

	itoa(task_stats.user_ticks, buff);
	write(1,"user_ticks ",11);
	write(1,buff,strlen(buff));

	itoa(task_stats.system_ticks, buff);
	write(1,"system_ticks ",13);
	write(1,buff,strlen(buff));

	itoa(task_stats.blocked_ticks, buff);
	write(1,"blocked_ticks ",15);
	write(1,buff,strlen(buff));

	itoa(task_stats.ready_ticks, buff);
	write(1,"ready_ticks ",12);
	write(1,buff,strlen(buff));

	total_time = task_stats.user_ticks + task_stats.system_ticks + task_stats.blocked_ticks + task_stats.ready_ticks;
	itoa(total_time, buff);
	write(1,"total time ",11);
	write(1,buff,strlen(buff));

}

// All processes CPU bounded
void workload1()  {
	int i;

	for (i=0; i<2; ++i) {

	}


}

// All processes I/O Bounded
void workload2()  {
	int i;

	for (i=0; i<2; ++i) {
		fork();
	}

	read(0,0,1024);
	stats(getpid());
	stats(0); // també del idle
}

// I/O bounded and CPU bounded
void workload3()  {
	int i;

	for (i=0; i<2; ++i) {
		
	}
}

// I/O bursts and CPU bursts
void workload4()  {
	int i;

	for (i=0; i<2; ++i) {
		
	}
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

    
   //Hauriem de tenir una manera de canviar entre RR i FCFS
	
  while(1) { }
}
