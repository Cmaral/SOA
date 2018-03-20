#include <asm.h>

ENTRY(assem_inner_task_switch)
	pushl %ebp
	movl %esp, %ebp
	movl 12(%ebp), %eax
	movl %ebp, (%eax)
	movl 8(%ebp), %esp
	popl %ebp
	ret