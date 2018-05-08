#include <libc.h>


char buff[24];

int sys_get_stats(int pid, struct stats *st);
void exit();

//Mostra per pantalla els stats del process amb pid task_pid
void stats(int task_pid) {
	int total_time;
	struct stats task_stats;
	get_stats(task_pid, &task_stats);


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
	int pid1 = 0;
	int n = 2;
	int m = 2;
	pid1 = fork();
	int pid2 = 0;
	if (pid1 == 0) {
			for (int i = 0; i < 1000000; i++) {
			if (n > 0) n = n*n;
		}
		stats(getpid());
		exit();
	}
	pid2 = fork();
	if (pid2 == 0) {
		for (int i = 0; i < 1000000; i++) {
			if (m > 0) m = m*m;
		}
		stats(getpid());
		exit();
	}
	exit();
}

// All processes I/O Bounded
void workload2()  {
	int pid1, pid2;
	pid1 = fork();
	if (pid1 == 0) {
		read(0,0,1024);
		stats(getpid());
		exit();
	}
	pid2 = fork();
	if (pid2 == 0) {
		read(0,2048,1024);
		stats(getpid());
		exit();
	}
	exit();
}

// I/O bounded and CPU bounded
void workload3()  {
	int pid1 = 0;
	int n = 2;
	int m = 2;
	pid1 = fork();
	int pid2 = 0;
	if (pid1 == 0) {
		for (int i = 0; i < 1000000; i++) {
			if (n > 0) n = n*n;
		}
		read(0,0,1024);
		n = 2;
		for (int i = 0; i < 1000000; i++) {
			if (n > 0) n = n*n;
		}
		stats(getpid());
		exit();
	}
	pid2 = fork();
	if (pid2 == 0) {
		for (int i = 0; i < 1000000; i++) {
			if (m > 0) m = m*m;
		}
		read(0,2048,1024);
		m = 2;
		for (int i = 0; i < 1000000; i++) {
			if (m > 0) m = m*m;
		}
		stats(getpid());
		exit();
	}
	exit();
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
	set_sched_policy(0);
	//workload1();
	//workload2();
	//workload3();
  while(1) { }
}
