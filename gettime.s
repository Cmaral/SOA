# 1 "gettime.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "gettime.S"
# 1 "include/asm.h" 1
# 2 "gettime.S" 2

.globl gettime; .type gettime, @function; .align 0; gettime:
        movl $10, %eax
        int $0x80
        cmpl $0, %eax
        jg fi
        imull $-1, %eax
        errno = %eax
        movl $-1, %eax
fi:
        iret
