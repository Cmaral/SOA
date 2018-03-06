#include <asm.h>

ENTRY(gettime)
	push %ebp 
	mov %esp,%ebp
        movl $10, %eax
        int $0x80
        cmpl $0, %eax
        jge fi
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi:
        pop %ebp
        ret
        
        
ENTRY(write)
        push %ebp
        mov %esp, %ebp
        movl 16(%ebp), %ebx
        movl 12(%ebp), %ecx
        movl 8(%ebp), %edx
        movl $4, %eax
        int $0x80
        cmpl $0, %eax
        jge fi2
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi2:
        pop %ebp
        ret