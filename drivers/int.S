.text

.macro ISR_NOERRCODE isr_num
  .global isr\isr_num
  isr\isr_num:
    cli
    push $\isr_num
    jmp isr_common_stub
.endm

.macro ISR_ERRCODE isr_num
  .global isr\isr_num
  isr\isr_num:
    cli
    push $0
    push $\isr_num
    jmp isr_common_stub
.endm

.macro IRQ irq_num, isr_num
  .global irq\irq_num
  irq\irq_num:
    cli
    push $0
    push $\isr_num
    jmp irq_common_stub
.endm

ISR_NOERRCODE isr_num=0
ISR_NOERRCODE isr_num=1
ISR_NOERRCODE isr_num=2
ISR_NOERRCODE isr_num=3
ISR_NOERRCODE isr_num=4
ISR_NOERRCODE isr_num=5
ISR_NOERRCODE isr_num=6
ISR_NOERRCODE isr_num=7
ISR_ERRCODE   isr_num=8
ISR_NOERRCODE isr_num=9
ISR_ERRCODE   isr_num=10
ISR_ERRCODE   isr_num=11
ISR_ERRCODE   isr_num=12
ISR_ERRCODE   isr_num=13
ISR_ERRCODE   isr_num=14
ISR_NOERRCODE isr_num=15
ISR_NOERRCODE isr_num=16
ISR_NOERRCODE isr_num=17
ISR_NOERRCODE isr_num=18
ISR_NOERRCODE isr_num=19
ISR_NOERRCODE isr_num=20
ISR_NOERRCODE isr_num=21
ISR_NOERRCODE isr_num=22
ISR_NOERRCODE isr_num=23
ISR_NOERRCODE isr_num=24
ISR_NOERRCODE isr_num=25
ISR_NOERRCODE isr_num=26
ISR_NOERRCODE isr_num=27
ISR_NOERRCODE isr_num=28
ISR_NOERRCODE isr_num=29
ISR_NOERRCODE isr_num=30
ISR_NOERRCODE isr_num=31
IRQ irq_num=0, isr_num=32
IRQ irq_num=1, isr_num=33
IRQ irq_num=2, isr_num=34
IRQ irq_num=3, isr_num=35
IRQ irq_num=4, isr_num=36
IRQ irq_num=5, isr_num=37
IRQ irq_num=6, isr_num=38
IRQ irq_num=7, isr_num=39
IRQ irq_num=8, isr_num=40
IRQ irq_num=9, isr_num=41
IRQ irq_num=10, isr_num=42
IRQ irq_num=11, isr_num=43
IRQ irq_num=12, isr_num=44
IRQ irq_num=13, isr_num=45
IRQ irq_num=14, isr_num=46
IRQ irq_num=15, isr_num=47

.extern isr_handler
isr_common_stub:
  pushal
  mov %ds, %ax
  push %eax
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  call isr_handler
  pop %ebx
  mov %bx, %ds
  mov %bx, %es
  mov %bx, %fs
  mov %bx, %gs
  popal
  add $4, %esp
  sti
  iret

.extern irq_handler
irq_common_stub:
  pushal
  mov %ds, %ax
  push %eax
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  call irq_handler
  pop %ebx
  mov %bx, %ds
  mov %bx, %es
  mov %bx, %fs
  mov %bx, %gs
  popal
  add $8, %esp
  sti
  iret
