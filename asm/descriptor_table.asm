
;This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();
global gdt_flush
extern gp

gdt_flush:
	lgdt [gp]
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	jmp 0x08:loop
loop:
	ret

; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'	
global idt_load
extern idtp

idt_load:
	lidt [idtp]
	ret

