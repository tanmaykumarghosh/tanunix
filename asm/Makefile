
include ../config/Make.config

objs = utility.o

asms = descriptor_table.o interrupt_request.o interrupt_routine.o

VPATH = ../obj/

default: all

all: $(objs) $(asms)


$(objs): %.o: %.c
	$(CC) $(CFLAGS) -c $<  -o ../obj/$@


$(asms): %.o: %.asm
	$(AS) $(ASFLAGS) -f elf -o ../obj/$@ $<
	
