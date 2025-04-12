
include ../config/Make.config


objs = kb.o scrn.o


VPATH = ../obj/


default: all


all: $(objs)


$(objs): %.o: %.c
	$(CC) $(CFLAGS) -c $<  -o ../obj/$@

