#ifndef __SYSTEM_H
#define __SYSTEM_H

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

/* MAIN.C */
extern unsigned char *memcpy(unsigned char *desc,const unsigned char *src,int count);
extern unsigned char *memset(unsigned char *desc,unsigned char val,int count);
extern unsigned short *memsetw(unsigned short *desc,unsigned short val,int count);
extern int strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port,unsigned char _data);
extern unsigned char inportb_p(unsigned short _port);
extern void outportb_p(unsigned short _port,unsigned char _data);
extern char *itos(char *s,long i);

/* SCRN.C */
extern void scroll(void);
extern void move_csr(void);
extern void cls();
extern void init_video(void);
extern void puts(unsigned char *str);
extern void putch(unsigned char c);
extern void putchtimer(unsigned char c);
extern void putstimer(unsigned char *text1,unsigned char *text2,unsigned char *text3,unsigned char *text4,unsigned char *text5,unsigned char *text6);

extern void gdt_set_gate(int num,unsigned long limit,unsigned long base,unsigned char access,unsigned char gran);
extern void gdt_install();

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();

/* IRQ.c */
extern void install_irq_handler(int no,void (*handler)(struct regs *r));
extern void uninstall_irq_handler(int no);
extern void irq_install();

/* TIMER.C */
extern void timer_install();



/* KEYBOARD.C */
extern void keyboard_install();
extern void sysbeepstop(void);



#endif
