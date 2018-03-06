#include <asm.h>

ENTRY(write_zeos_ticks)
        movl $9, %eax
        int $0x80
        cmpl $0, %eax
        jg fi
        imull $-1, %eax
        errno = %eax
        movl $-1, %eax
fi:
        iret