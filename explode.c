#include "bithdwr.h"
#include "bg.h"
#include "style.h"
#include "pacman.h"

#define NPIECES 80
#define ROW_OFFSET 0
#define RADIUS 100
#define STEPS 10
struct piece {
	int xdist, ydist;
	int xpos, ypos;
	int xinit, yinit;
} piece[NPIECES];

explode(r,c)
int r,c;
{
	int  s;
	register int initx;
	register int inity;
	int destx;
	int desty;
	register struct piece *p;
	register int  i;

	initx = c;
	inity = r;
	for ( i=0 ; i<NPIECES ; i++ ) {
		p = &piece[i];
		do {
			destx = grand((int)-RADIUS,(int)RADIUS);
			desty = grand((int)-RADIUS,(int)RADIUS);
		} while ( destx*destx+desty*desty > RADIUS*RADIUS);
		destx += initx;
		desty += inity;
		p->xdist = destx-initx;
		p->ydist = desty-inity;
		p->xinit = initx;
		p->yinit = inity;
		p->xpos = p->xinit;
		p->ypos = p->yinit;
	}
	fruitsound();
	rsetdead();
	for ( i=0 ; i<NPIECES ; i++ )
		turnon(&piece[i]);
	for ( s=0 ; s<STEPS+1 ; s++ ) {
		for ( i=0 ; i<NPIECES ; i++ ) {
			p = &piece[i];
			if ( s!=0 )
				turnoff(p);
			p->xpos = p->xinit + p->xdist*s/STEPS;
			p->ypos = p->yinit + p->ydist*s/STEPS;
			turnon(p);
		}
		rsetdead();
	}
	for ( i=0 ; i<NPIECES ; i++ )
		turnoff(&piece[i]);
}
char Font[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

turnon(p)
register struct piece *p;
{
	register unsigned char *base, *end;
	register char *font;
	int r, c;
	c = p->xpos - 2;
	r = p->ypos + 2;
	if ( p->xpos >= 96 || p->xpos < 0 ||
	     p->ypos >= 128 || p->ypos < 0 ) return;

	base = SCtoMEM(r, c);
	end = base + 128*8;
	font = Font;
	for ( ; base<end ; base += 128 )
		*base ^= *font++;
}
turnoff(p)
register struct piece *p;
{
	register unsigned char *base, *end;
	register char *font;
	int r, c;
	c = p->xpos - 2;
	r = p->ypos + 2;
	if ( p->xpos >= 96 || p->xpos < 0 ||
	     p->ypos >= 128 || p->ypos < 0 ) return;

	base = SCtoMEM(r, c);
	end = base + 128*8;
	font = Font;
	for ( ; base<end ; base += 128 )
		*base ^= *font++;
}
