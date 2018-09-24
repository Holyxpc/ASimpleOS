.section ".text", "ax"

.global start32,stack_end

start32:
	movl 	$(stack_end), %esp
	call	start_kernel
	hlt
	.align (1 << 12)
stack:
	.fill 1024, 4, 0
stack_end:
