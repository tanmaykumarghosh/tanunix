/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Keyboard driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* theser are used for beep sound */
int beepcount = 0;
/* these for screen x and y axis value from  scrn.c file */
extern int csr_x;
extern int csr_y;

/* this part actuall  need to place in the  timer file section as per given tutorial accessing 0x43 port  for easy calling but I have added here in kb secction as keyboard b switch for beep and s switch for stop beep */
/* stop beep if s switch press */
void sysbeepstop(void)
{
	/* disable counter 2 */
	outportb( 0x61,inportb_p(0x61)&0xFC);
}
/* start beep if b switch press */
static void sysbeep(void)
{
	/* enable counter 2 */
	outportb_p( 0x61,inportb_p(0x61)|3);
	/* set command for counter 2, 2 byte write */
	outportb_p( 0x43,0xB6);
	/* send 0x637 for 750 HZ */
	outportb_p( 0x42,0x37);
	outportb( 0x42,0x06);
	/* 1/8 second */
	beepcount = 100/8;	
}

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */


unsigned char kbdus[128]={
 0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void keyboard_handler(struct regs *r){
 	unsigned char scancode;
	/* Read from the keyboard's data buffer */
	scancode = inportb(0x60);

	/* If the top bit of the byte we read from the keyboard is
		*  set, that means that a key has just been released */
	if(scancode & 0x80)
	{
		/* You can use this one to see if the user released the
			*  shift, alt, or control keys... */
	}else{
		/* Here, a key was just pressed. Please note that if you
			*  hold a key down, you will get repeated key press
			*  interrupts. */

			/* Just to show you how this works, we simply translate
			*  the keyboard scancode into an ASCII value, and then
			*  display it to the screen. You can get creative and
			*  use some flags to see if a shift is pressed and use a
			*  different layout, or you can add another 128 entries
			*  to the above layout to correspond to 'shift' being
			*  held. If shift is held using the larger lookup table,
			*  you would add 128 to the scancode when you look for it */
		/* this displaying key in the screen */
		putch(kbdus[scancode]);
		/* this portion is for beep sound keypress check */
		if(kbdus[scancode]=='s')
		{
			sysbeepstop();
		}else if(kbdus[scancode]=='b')		{
			sysbeep();
			puts("\npress s to stop the beep\n");
		}
		/* uncomment this section if you want to clear the screen pressing c */
		/*
		if(kbdus[scancode]=='c')
		{
		cls();
		putch('#');
		}
		*/
	}
}
/* Installs the keyboard handler into IRQ1 */
void keyboard_install(){
	install_irq_handler(1,keyboard_handler);
}
