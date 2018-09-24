.text

.global switch_to

switch_to:
	movl    4(%esp),%eax
    movl    %esp,0(%eax)
    movl    %ebp,4(%eax)     
    movl    %ebx,8(%eax)
    movl    %esi,12(%eax)
    movl    %edi,16(%eax)
    pushfl
    popl    %ecx
    movl    %ecx,20(%eax)

    movl    8(%esp),%eax
    movl    0(%eax),%esp
    movl    4(%eax),%ebp
    movl    8(%eax),%ebx
    movl    12(%eax),%esi
    movl    16(%eax),%edi
    movl    20(%eax),%eax
    pushl   %eax
    popfl

    ret