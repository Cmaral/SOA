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
