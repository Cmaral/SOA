# 1 "assem_inner_task_switch.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "assem_inner_task_switch.S"
# 1 "include/asm.h" 1
# 2 "assem_inner_task_switch.S" 2

.globl assem_inner_task_switch; .type assem_inner_task_switch, @function; .align 0; assem_inner_task_switch:
 pushl %ebp
 movl %esp, %ebp
 movl 12(%ebp), %eax
 movl %ebp, (%eax)
 movl 8(%ebp), %esp
 popl %ebp
 ret
