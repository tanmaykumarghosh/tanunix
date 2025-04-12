/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */

#include<system.h>
/* get the timer structure */
#include<time.h>
/* initialize timer variables used in timer c file*/
char *ampm;
int hours,temphours;
int minutes;
int seconds;
int day;
int month;
int year;

/*
 * Yeah, yeah, it's ugly, but I cannot find how to do this correctly
 * and this seems to work. I anybody has more info on the real-time
 * clock I'd be interested. Most of this was trial and error, and some
 * bios-listing reading. Urghh.
 */
/* this code read from cmos time used in old days linux source code */

#define CMOS_READ(addr) ({ \
outportb_p(0x70,0x80|addr); \
inportb_p(0x71); \
})

#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

static void time_init(void)
{
	struct tm time;

	do {
		time.tm_sec = CMOS_READ(0);
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	} while (time.tm_sec != CMOS_READ(0));
	BCD_TO_BIN(time.tm_sec);
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
	time.tm_mon--;
	
	
	hours = time.tm_hour;
	// hours = time.tm_hour;
	minutes = time.tm_min;
	seconds = time.tm_sec;
	day = time.tm_mday;
	month = time.tm_mon;
	year = time.tm_year;
	if(time.tm_hour >= 12)
	ampm = "PM";
	else
	ampm ="AM";
}




void main()
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_video();
	timer_install();
	/* get actual time from bios */
	time_init();

	keyboard_install();
	puts("TanUnix 0.02 Version Welcome to My Unix World\n");
	
        __asm__ __volatile__("sti");
	for(;;);
}
