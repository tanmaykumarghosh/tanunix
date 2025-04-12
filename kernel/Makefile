
include ../config/Make.config

objs = main.o gdt.o idt.o irq.o isrs.o timer.o

asms = 

VPATH = ../obj/

default: all

all: $(objs) $(asms)


$(objs): %.o: %.c
	$(CC) $(CFLAGS) -c $<  -o ../obj/$@


$(asms): %.o: %.s
	$(AS) $(ASFLAGS) -f elf -o ../obj/$@ $<
	

