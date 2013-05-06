/*	PIECES COMMENTED OUT TO SAVE SPACE */


#include "compat.h"

/*VARARGS1*/
printf(fmt, x1)
char *fmt;
unsigned x1;
{
    extern int showchar();

    doprf(fmt, &x1, showchar);
}


/*
 * Scaled down version of C Library printf.
 */

doprf(fmt, adx, pfunc)
register char *fmt;
register int  *adx;
int          (*pfunc)();
{
	register int b, c, i, len;
	char *s;
	int zfill;
	int any;
	long number;

loop:
	while ((c = *fmt++) != '%') {
		if(c == '\0')
			return;
		(*pfunc)(p1(c));
	}
	len = 0;
	zfill = 0;

again:
	c = *fmt++;

	switch (c) {

	case 'r':
		adx = *(int **)(adx);
		fmt = *(char **)(adx)++;
		break;

	case 'D':
	case 'd':
		if (c == 'D')
		    number = *(long *)(adx)++;
		else
		    number = *(int *)(adx)++;

		b = 10;
		printn(number, b, len, zfill, pfunc);
		break;
	case 's':
		s = *(char **)(adx)++;
		while (c = *s++)
			(*pfunc)(p1(c));
		break;

	case '0':
		if ( len==0 ) zfill = 1;
	case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		len = len*10 + (c-'0');
		goto again;




	case '%':
		(*pfunc)(p1('%'));
		break;
	}
	goto loop;
}

/*
 * Printn prints a number n in base b.
 */
printn(n, b, len, zfill, pfunc)
register unsigned long n;
register int len;
int (*pfunc)();
{
	char prbuf[11];
	register char *cp;
	int ch;

	if (b == 10 && (int)n < 0) {
		(*pfunc)(p1('-'));
		n = (unsigned)(-(int)n);
	}
	cp = prbuf;
	do {
		*cp++ = "0123456789abcdef"[n%b];
		n /= b;
	} while (n);
	while ( len-- > cp-prbuf )
		if ( zfill )
			(*pfunc)(p1('0'));
		else
			(*pfunc)(p1(' '));

	do {
		ch = *--cp;
		(*pfunc)(p1(ch));
	} while (cp > prbuf);
}


