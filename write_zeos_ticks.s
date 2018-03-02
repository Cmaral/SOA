# 1 "write_zeos_ticks.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "write_zeos_ticks.S"
# 1 "include/asm.h" 1
# 2 "write_zeos_ticks.S" 2

.globl write_zeos_ticks; .type write_zeos_ticks, @function; .align 0; write_zeos_ticks:
        movl $9, %eax
        int $80
        cmpl $0, %eax
        jg fi
        imull $-1, %eax
        errno = %eax
        movl $-1, %eax
fi:
        iret
