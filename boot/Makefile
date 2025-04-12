
include ../config/Make.config

VPATH = ../obj/

default: all

all: start.o


start.o: start.asm
	$(AS) $(ASFLAGS) -f elf -o start.o start.asm

