/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Screen output functions for Console I/O
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* these variables are for display timer extern from main c file */
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


/* I have added the color code for foreground color and background color actually use correct color code either screen will be flick */

#define COLOR_BLACK  0x00
#define COLOR_BLUE    0x01
#define COLOR_GREEN  0x02
#define COLOR_CYAN    0x03
#define COLOR_RED      0x04
#define COLOR_MAGENTA   0x05
#define COLOR_BROWN  0x06
#define COLOR_LTGRAY    0x07
#define COLOR_DKGRAY    0x08
#define COLOR_LTBLUE    0x09
#define COLOR_LTGREEN   0x0A
#define COLOR_LTCYAN    0x0B
#define COLOR_LTRED  0x0C
#define COLOR_LTMAGENTA 0x0D
#define COLOR_LTBROWN   0x0E
#define COLOR_WHITE  0x0F



/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;
int temp_csr_x = 0, temp_csr_y = 0,temp_scroll_csr_x=0;

/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    //blank = 0x20 | (attrib << 8);
    blank = 0x20 | (settextcolor(COLOR_BLUE, COLOR_BLUE) << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
	putstimer(itos(str1,temphours),":",itos(str2,minutes),":",itos(str3,seconds),ampm);
	
	if(temp_scroll_csr_x>=80)
	{
	}
	else
	{
	csr_x=0;
	putch('#');
	temp_scroll_csr_x=0;
	}
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    

    temp = csr_y * 80 + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    //blank = 0x20 | (attrib << 8);
      blank = 0x20 | (settextcolor(COLOR_BLUE, COLOR_BLUE) << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
    unsigned short *where;
   /* unsigned att = attrib << 8;*/
   unsigned att = settextcolor(COLOR_GREEN, COLOR_BLUE) << 8;


    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
		temp_scroll_csr_x=0;
        csr_y++;
	
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = '#'| att;	/* Character AND attributes: color */
        csr_x++;
    }
   
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
		temp_scroll_csr_x=csr_x;
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
   if(csr_x==0)
   csr_x++;
   
   scroll();
    move_csr();
}
/* I have added this function display timer top right side of the screen */
void putchtimer(unsigned char c)
{
    unsigned short *wheretimer;
    unsigned atttimer = (settextcolor(COLOR_MAGENTA, COLOR_BLUE) << 8);
   
   
    wheretimer = textmemptr + (csr_y * 80 + csr_x);
    *wheretimer = c | atttimer;	
	
	
     move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}
/* I have added this function to display timer in the right top of the screen it's not well format I used here for first time */
 void putstimer(unsigned char *text1,unsigned char *text2,unsigned char *text3,unsigned char *text4,unsigned char *text5,unsigned char *text6)
{
    int i;
	temp_csr_x=csr_x;
	temp_csr_y=csr_y;
	csr_x=70;
	csr_y=0;
    for (i = 0; i < strlen(text1); i++)
    {
        putchtimer(text1[i]);
		csr_x++;
    }
    for (i = 0; i < strlen(text2); i++)
    {
        putchtimer(text2[i]);
		csr_x++;
    }
    for (i = 0; i < strlen(text3); i++)
    {
        putchtimer(text3[i]);
		csr_x++;
    }
   for (i = 0; i < strlen(text4); i++)
    {
        putchtimer(text4[i]);
		csr_x++;
    }
   for (i = 0; i < strlen(text5); i++)
    {
        putchtimer(text5[i]);
		csr_x++;
    }
    for (i = 0; i < strlen(text6); i++)
    {
        putchtimer(text6[i]);
		csr_x++;
    }
	csr_x=temp_csr_x;
	csr_y=temp_csr_y;
	move_csr();
}


/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
