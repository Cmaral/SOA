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
        pushl %ebx
        pushl %ecx
        pushl %edx
        movl 8(%ebp), %ebx
        movl 12(%ebp), %ecx
        movl 16(%ebp), %edx
        movl $4, %eax
        int $0x80
        cmpl $0, %eax
        jge fi2
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi2:
        popl %edx
        popl %ecx
        popl %ebx
        pop %ebp
        ret
        
ENTRY(getpid)
        push %ebp
        movl %esp, %ebp
        movl $20, %eax
        int $0x80
        cmpl $0, %eax
        jge fi3
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi3:
        pop %ebp
        ret
        
ENTRY(fork)
        push %ebp
        movl %esp, %ebp
        movl $2, %eax
        int $0x80
        cmpl $0, %eax
        jge fi4
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi4:
        pop %ebp
        ret

ENTRY(exit)
        push %ebp
        movl %esp, %ebp
        movl $1, %eax
        int $0x80
        cmpl $0, %eax
        jge fi5
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi5:
        pop %ebp
        ret
        
        
ENTRY(get_stats)
        push %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %ecx
        movl 8(%ebp), %ebx
        movl 12(%ebp), %ecx
        movl $35, %eax
        int $0x80
        cmpl $0, %eax
        jge fi6
        imull $-1, %eax
        movl %eax, errno
        movl $-1, %eax
fi6:
        popl %ecx
        popl %ebx
        pop %ebp
        ret