#include <asm.h>

ENTRY(assem_inner_task_switch)
	movl 8(%esp), %eax
	movl %ebp, (%eax)
	movl 4(%esp), %esp
	popl %ebp
	ret