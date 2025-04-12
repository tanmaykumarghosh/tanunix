include config/Make.config

KERNEL = kernel
SUBDIRS = boot asm kernel drivers

.PHONY: subdirs $(SUBDIRS)

default: all


all: subdirs linking


subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@


all:subdirs linking

clean:
	@echo "removing temp files now...."
	@rm -rf obj/*.o boot/*.o asm/*.o  drivers/*.o apps/*.o
	@rm -f *.bin
	@rm -f kernel.img
	@echo "     Done..."

linking:
	@echo "building modules..."
	
	
	@$(LD) $(LDFLAGS_LINK) $(LDFLAGS) -o $(KERNEL).bin boot/start.o obj/*.o
	@echo "     Done..."

image:
	@echo "creating boot image..."
	@dd if=$(KERNEL).bin of=$(KERNEL).img bs=512 count=1 $(REDIRECT)
	@dd if=/dev/zero of=$(KERNEL).img skip=1 seek=1 bs=512 count=2879 $(REDIRECT)
	@echo "     Done..."

setup:
	@echo "setting up the environment..."
	
	@mkdir -p testmnt;
	@sudo mount -o loop tanmay_kernel_grub.img ./testmnt;
	@sudo cp ./$(KERNEL).bin ./testmnt;
	@sudo umount ./testmnt;
	@sudo rm -rf ./testmnt;
	@echo "     Done..."

test:
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	make clean
	make all
	make image
	make setup
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	qemu-system-i386 -fda tanmay_kernel_grub.img
