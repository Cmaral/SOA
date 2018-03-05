/*
 * libc.c 
 */

#include <libc.h>

#include <types.h>

int errno;
void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

int gettime() {
        __asm__("movl $10, %%eax;"
            "int $0x80;"
            :"=a"(errno)    
            :   
            :"%eax"     
        );
        
        if (errno == 0) return errno;
        return (-1*errno);
}

int write_zeos_ticks(int valor) {
        __asm__("movl $9, %%eax;"
            "movl %1, %%ebx;"
            "int $0x80;"
            :"=a"(errno)    
            :"r"(valor)     
            :"%eax"     
        );  
        
        if (errno == 0) return errno;
        return (-1*errno);
}





