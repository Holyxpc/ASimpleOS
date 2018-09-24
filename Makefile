TARGET=bin/vmlinux

CC=gcc
LD=ld
CFLAGS=-m32 -fno-stack-protector -fno-builtin -nostdlib -g -static -Wall
LDFLAGS=-m elf_i386

INCLUDE=-I./boot/include -I./include -I./include/sys

ISOIMAGE=bin/isoimage.iso
GRUB_CONF=config/grub.cfg

.PHONY:all clean disasm install run
	
all:$(OBJS) $(TARGET) 

SRC_CC=$(wildcard *.c kernel/*.c)
SRC_CC+=$(wildcard *.c mm/*.c)
SRC_CC+=$(wildcard *.c lib/*.c)
SRC_CC+=$(wildcard *.c boot/*.c)
SRC_CC+=$(wildcard *.c video/*.c)
SRC_CC+=$(wildcard *.c init/*.c)
SRC_CC+=$(wildcard *.c drivers/*.c)

SRC_ASM=$(wildcard *.S kernel/*.S)
SRC_ASM+=$(wildcard *.S boot/*.S)
SRC_ASM+=$(wildcard *.S lib/*.S)
SRC_ASM+=$(wildcard *.S drivers/*.S)



OBJS=$(patsubst %c, %o, $(SRC_CC))
OBJS+=$(patsubst %S, %o, $(SRC_ASM))


%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)
	@echo "\tCC $< to $@ .... [ \033[40;32mOK\033[0m ]"

%.o: %.S
	@$(CC) -o $@ -c $^ $(CFLAGS) $(INCLUDE)
	@echo "\tASM $< to $@ .... [ \033[40;32mOK\033[0m ]"



$(TARGET):$(OBJS) 
	@$(LD)  -N -o $@ $^ -T vmlinux.lds
	@echo "\tLINK ${OBJS} to $@ .... [ \033[40;32mOK\033[0m ]"
	@echo "\tBuild date `date`"


clean:
	@rm -rf $(TARGET) $(OBJS)  kernel.asm iso/ $(ISOIMAGE)
	@echo "\tCLEAN  .... [ \033[40;32mOK\033[0m ]"
disasm:
	@objdump -S $(TARGET) > kernel.asm
	@echo "\tCREAT kernel.asm .... [ \033[40;32mOK\033[0m ]"

isoimage:$(GRUB_CONF) $(TARGET)
	@mkdir -p iso/boot/grub
	@cp config/grub.cfg iso/boot/grub
	@cp -rf /usr/lib/grub/i386-pc/ iso/boot/grub/
	@cp $(TARGET) iso/boot
	@grub-mkrescue -o $(ISOIMAGE) iso
	@rm -rf iso/

#run:$(ISOIMAGE)
#isoimage:$(GRUB_CONF) $(TARGET)
run:$(GRUB_CONF) $(TARGET)
	@mkdir -p iso/boot/grub
	@cp config/grub.cfg iso/boot/grub

	@cp -rf bin/i386-pc/ iso/boot/grub/
	#@cp -rf /usr/lib/grub/i386-pc/ iso/boot/grub/
	@cp $(TARGET) iso/boot
	@grub-mkrescue -o $(ISOIMAGE) iso
	@rm -rf iso/
	@qemu-system-i386 -cdrom $(ISOIMAGE)
