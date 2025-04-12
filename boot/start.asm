[BITS 32]

section .__mbHeader

; This part MUST be 4byte aligned, so we solve that issue using 'ALIGN 4'
;ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    extern code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

section .data
	message db 'Hello, World!', 0  ; The string to print, null-terminated


global start
start:
    extern main
   

	mov esp, _sys_stack     ; This points the stack to our new stack area
	call main
    jmp $

    ; Hang the system (infinite loop)
hang:
    cli
    hlt
    jmp hang
   

; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'

section .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:

