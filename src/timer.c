/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Timer driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks =0;

/* these all ar for display time extern from main c file to display clock */
extern char str1[2];
extern char str2[2];
extern char str3[2];
extern char *ampm;
extern int hours;
extern int temphours;
extern int minutes;
extern int seconds;
extern int day;
extern int month;
extern int year;
/* this is screen x an y variable scrn.c file */
extern int csr_x;
extern int csr_y;

extern char *scanbuffer;

/* acutally this is an implementaion not working in this code */
void timer_phrase(int hz){
	int divisor = 1193180 / hz;
	outportb(0x43,0x36);
	outportb(0x40, (divisor & 0xFF));
	outportb(0x40,(divisor >> 8));
}

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct regs *r){
	timer_ticks++;


	if(timer_ticks % 18 == 0){
	/* puts("one second gone\n");  */
      
        /* I have enhanced this section to display timer per second interval */
	if(seconds>59)
	{
	seconds=0;
	minutes++;
	}
	if(minutes>59)
	{
	minutes=0;
	hours++;
	}

	if(hours>23)
	hours=00;

	if(hours>12)
	temphours = hours-12;
	else
	temphours = hours;

	if(hours >= 12)
	ampm = "PM";
	else
	ampm ="AM";

	putstimer(itos(str1,temphours),":",itos(str2,minutes),":",itos(str3,seconds),ampm);
	seconds++;


	}

}


/* This will continuously loop until the given time has
*  been reached */
/* this is just implementaion not working here */
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}


void timer_install(){
	/* timer_phrase(100); */
	install_irq_handler(0,timer_handler);
}


