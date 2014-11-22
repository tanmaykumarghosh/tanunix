.SILENT:
NASM		  = nasm
NASM_FLAGS        = -O99 -f elf32 
CC                = gcc
LD                = ld
CFLAGS            = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include
LDFILE_LINK    = link.ld
LDFLAGS_LINK   = -T $(LDFILE_LINK)
DD                = dd
START              = start
KERNEL            = kernel
REDIRECT          = > /dev/null 2>&1

clean:
	@echo "removing temp files now...."
	@rm -rf *.o
	@rm -f *.bin
#	@rm -f *.img
	@echo "     Done..."

compile:
	@echo "building modules..."
	@$(NASM) $(NASM_FLAGS) -o $(START).o $(START).asm 
	@$(CC) $(CFLAGS) main.c -c -o main.o
	@$(CC) $(CFLAGS) scrn.c -c -o scrn.o
	@$(CC) $(CFLAGS) gdt.c -c -o gdt.o
	@$(CC) $(CFLAGS) idt.c -c -o idt.o
	@$(CC) $(CFLAGS) isrs.c -c -o isrs.o
	@$(CC) $(CFLAGS) irq.c -c -o irq.o
	@$(CC) $(CFLAGS) timer.c -c -o timer.o
	@$(CC) $(CFLAGS) kb.c -c -o kb.o
	
	
	@$(LD) $(LDFLAGS_LINK) -melf_i386 -o $(KERNEL).bin start.o main.o scrn.o gdt.o idt.o isrs.o irq.o timer.o kb.o 
	@echo "     Done..."

image:
	@echo "creating boot image..."
	@dd if=$(KERNEL).bin of=$(KERNEL).img bs=512 count=1 $(REDIRECT)
	@dd if=/dev/zero of=$(KERNEL).img skip=1 seek=1 bs=512 count=2879 $(REDIRECT)
	@echo "     Done..."

setup:
	@echo "setting up the environment..."
	@mkdir -p testmnt;
	@mount -o loop tanmay_kernel_grub.img ./testmnt;
	@cp ./$(KERNEL).bin ./testmnt;
	@umount ./testmnt;
	@rm -rf ./testmnt;
	@echo "     Done..."

test:
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	make clean
	make compile
	make image
	make setup
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
