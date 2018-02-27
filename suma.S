#include <asm.h>

ENTRY (add2)
push %ebp; \
mov %esp,%ebp; \
mov 8(%ebp),%edx; \
mov 12(%ebp),%eax; \
add %edx,%eax; \
pop %ebp; \
ret; \
nop
