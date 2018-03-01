#include <asm.h>

ENTRY(gettime)
        movl $10, %eax
        int 0x80
        cmpl $0, %eax
        jg fi
        imull $-1, %eax
        errno = %eax
        movl $-1, %eax
fi:
        iret