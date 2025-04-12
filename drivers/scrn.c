/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Screen output functions for Console I/O
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>
#define VGA_BADR 0xB8000 
#define CR 0xD
#define LF 0xA
#define MONO 0x0F20
/* these variables are for display timer extern from main c file */
extern char *ampm;
extern int hours;
extern int minutes;
extern int seconds;



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
//unsigned short int *textmemptr;
unsigned short int *textmemptr = ((unsigned short int*)VGA_BADR);
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
       // memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        memsetw(textmemptr+ (25 - temp) * 80, 0x0F20, 80); 
        csr_y = 25 - 1;
        if(temp_scroll_csr_x>=80)
        {
        
        csr_x=0;
        putch('#');
        temp_scroll_csr_x=0;
        }
    }
}

unsigned short int pixel_offset(unsigned short int x, unsigned short int y) {
    if ((0 < x && x <= 80) && (0 < y && y <= 25)) {
      return ((y-1) * 80) + x;
    }
    return 1;
}
/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    int temp;
    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */    

    temp = pixel_offset(csr_x, csr_y); //csr_y * 80 + ;

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

void cls() {
    // clear out the screen
    unsigned blank;
    int i = 0;
    blank = 0x20 | (settextcolor(COLOR_BLUE, COLOR_BLUE) << 8);
    csr_x = 1;
    csr_y = 1;
    move_csr();
    while (i++ < 80) {
      memsetw(textmemptr, MONO, 25); 
      textmemptr += 25;
    }
    textmemptr = ((unsigned short int*)VGA_BADR);
}

void putch(char c) {
    unsigned short int *where;
    // put character at the current cursor posit. 1-indexed
    unsigned short int loc = pixel_offset(csr_x, csr_y); 
    textmemptr = ((unsigned short int*)VGA_BADR) + loc;
    unsigned short int attribute = COLOR_GREEN << 4 | COLOR_BLUE & 0xF;
    // TODO cursor CRLF and scroll
    // /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    //}
    // /* Handles a tab by incrementing the cursor's x, but only
    // *  to a point that will make it divisible by 8 */
    // else if(c == 0x09){
    //     csr_x = (csr_x + 8) & ~(8 - 1);
    /*handle spacebar*/
    // }else if(c >= ' '){
    //     //textmemptr = textmemptr + (csr_y * 80 + csr_x);
    //     csr_x++;
    //     // move_csr();
    //     // return;
    }else  
    if (c == LF || c == '\n') {   
      csr_x = 0;
      csr_y++;
      putch('T');
      return;
    }else if(c == CR || c == '\r') {
      csr_x = 0;  
      csr_y++;   
      return;
    }else if(csr_x > 80){
        csr_x = 0;
        csr_y++;
        // move_csr();
        // return;
    }else if(csr_x >= 80)
    {
    	temp_scroll_csr_x=csr_x;
        csr_x = 0;
        csr_y++;
    }else{
      csr_x++;
    }
    *textmemptr = c | (attribute << 8);
    move_csr();
    // so this *CURPIX is pointing directly to a memory address.
    // changing the value at the memory address for a pixel means
    // that we directly change the way its rendered, thus we do 
    // not need any additional interrupts than the "hardware"
    // clock itself.
}
/* I have added this function display timer top right side of the screen */
void putchtimer(char c)
{
    unsigned short int *wheretimer;
    //unsigned atttimer = (settextcolor(COLOR_MAGENTA, COLOR_BLUE) << 8);
    unsigned short int atttimer = COLOR_MAGENTA << 4 | COLOR_BLUE & 0xF;
    //unsigned short int loc = pixel_offset(csr_x, csr_y); 
    wheretimer = ((unsigned short int*)VGA_BADR) + (csr_y * 80 + csr_x);
    //wheretimer = textmemptr + (csr_y * 80 + csr_x);
    *wheretimer = c | (atttimer << 8);		
     move_csr();
}

void puts(char* s) {
    char c;
    while (c = *s++) {
        putch(c);
    }
  }
/* I have added this function to display timer in the right top of the screen it's not well format I used here for first time */
 void putstimer(char *hour,char *fcolon,char *minute,char *scolon,char *second,char *ampm)
{
    int i;
    temp_csr_x=csr_x;
    temp_csr_y=csr_y;
    csr_x=70;
    csr_y=0;
    for (i = 0; i < strlen(hour); i++)
    {
        putchtimer(hour[i]);
		    csr_x++;
    }
    for (i = 0; i < strlen(fcolon); i++)
    {
        putchtimer(fcolon[i]);
		    csr_x++;
    }
    for (i = 0; i < strlen(minute); i++)
    {
        putchtimer(minute[i]);
		    csr_x++;
    }
    for (i = 0; i < strlen(scolon); i++)
    {
        putchtimer(scolon[i]);
		    csr_x++;
    }
   for (i = 0; i < strlen(second); i++)
    {
        putchtimer(second[i]);
		    csr_x++;
    }
    for (i = 0; i < strlen(ampm); i++)
    {
        putchtimer(ampm[i]);
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
    cls();
}
