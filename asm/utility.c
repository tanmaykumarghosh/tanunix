#include<system.h>
unsigned char *memcpy(unsigned char *desc,const unsigned char *src,int count)
{
	const char *sc = (unsigned char *)src;
	unsigned char *de = (unsigned char *)desc;
	for(;count!=0;count--) *de++ = *sc++;
	return desc;
	
}
void memcpyw(unsigned short int* des, unsigned short int* source, int count) {
	while (count-- > 0) {
	  des[count] = source[count];
	}
  }

unsigned char *memset(unsigned char *desc,unsigned char val,int count)
{
    unsigned char *temp = (unsigned char *)desc;
    for( ; count != 0; count--) *temp++ = val;
    return desc;
}
unsigned short *memsetw(unsigned short *desc,unsigned short val,int count)
{
	unsigned short *temp = (unsigned short *)desc;
	for(;count!=0;count--) *temp++=val;
	return desc;
}
int strlen(unsigned char *str)
{
	int retval;
	for(retval=0;*str != '\0';str++) retval++;
	return retval;
}

unsigned char inportb(unsigned short _port)
{
 unsigned char rv;
 __asm__ __volatile__ ("inb %1,%0" : "=a" (rv) : "dN" (_port));
 return rv;
}

/* enhance the further inport function */
unsigned char inportb_p(unsigned short _port)
{
 unsigned char rv;
 __asm__ __volatile__ ("inb %%dx,%%al\n" \
			"\tjmp 1f\n" \
			"1:\tjmp 1f\n" \
			"1:":"=a" (rv):"d" (_port));
 return rv;
}

void outportb(unsigned short _port,unsigned char _data)
{
__asm__ __volatile__ ("outb %1,%0" : : "dN" (_port), "a" (_data));
}

/* enhance the further outport function */
void outportb_p(unsigned short _port,unsigned char _data)
{
__asm__ __volatile__ ("outb %%al,%%dx\n" \
			"\tjmp 1f\n" \
			"1:\tjmp 1f\n" \
			"1:"::"a" (_data),"d" (_port));
}

/* this itos function implementation without any library convert a inter or long value to stings variables */
char *itos(char *s,long i)
{ 
	char *r = s;
	long t;
	if (i < 10)
	{ 
	*s++ = '0';
	}

	t = i;
	do ++s; while (t /= 10);
	*s = '\0';
	do *--s = '0' + i % 10; while (i /= 10);
	return r;
}

