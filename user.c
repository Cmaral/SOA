#include <libc.h>
#include <stats.h>

int add2(int par1, int par2);

int gettime();

int getpid();

int fork();

char buff[24];

int pid;

int get_stats(int pid, struct stats *st);

long inner (long n)
{
	int i;
	long suma;
	suma = 0;
	for (i=0; i<n; i++) suma = suma + i;
		return suma;

}

long outer (long n) {
	int i;
	long acum;
	acum = 0;
	for (i=0; i<n; i++) acum = acum + inner(i);
		return acum;
}

int add(int par1, int par2) {
	return(par1 + par2);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
     

    struct stats st;
    char* buffer = "";
    buffer = "";

    int pid1 = fork();
    /*itoa(pid1, buffer);
    write(1, buffer, strlen(buffer));
    while (getpid() == 11) {
      write (1, "hola", 4);
    }
    while (pid1 == 11) {
      write (1, "ADEU", 4);
    }*/
    
   perror();
   while (1);
   return 0;
}
