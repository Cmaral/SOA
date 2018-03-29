# 1 "wrapper.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "wrapper.S"
# 1 "include/asm.h" 1
# 2 "wrapper.S" 2

.globl gettime; .type gettime, @function; .align 0; gettime:
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


.globl write; .type write, @function; .align 0; write:
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

.globl getpid; .type getpid, @function; .align 0; getpid:
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

.globl fork; .type fork, @function; .align 0; fork:
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
