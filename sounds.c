/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File:	sounds.c
 *	      Contents: Tone generator routines
 *	        Author: Bob Brown (rlb)
 *	   Description:	All routines that access the tone generator.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

#include "style.h"
#include "pacman.h"
#include "compat.h"
#include "bithdwr.h"

/*
 * Never liked the results of this module.
 */

int tonegen(), snd_dot();
char enable = 077;

/*
 * sound when a dot is et.  goes DEE-DUM
 */

int
sounddot()
{
	if ( Silent ) return;
	snd_dot(NULL,0);
}
int
snd_dot(junk,part)
char *junk;
int part;
{
	switch(part){
	case 0: /* DEE */
		tonegen(1,1200,12);
		eladd(SNDDOTDELAY,snd_dot,NULL,1);
		break;
	case 1: /* DUM */
		tonegen(1,1000,15);
		eladd(SNDDOTDELAY,snd_dot,NULL,2);
		break;
	case 2:
		tonegen(1,0,0);
	}
}
/*
 * This is the sound when a monster is killed
 */
#define KILLFROM 200
#define KILLTO	50

soundkill()
{
	tonegen(1,KILLFROM,13);
	PsgSweep(0,KILLFROM,KILLTO,5);
	tonegen(1,0,0);
}

/*
 * Access a single tone generator.
 *
 * Could really use some constants and mnemonics here.
 */

int
tonegen(t,p,v)
register t,p,v;
{

	v *= VolumeControl;                     /* Control volume.  */
	v /= 16;

	if ( p > 0 ) {
		enable &= ~(1<<(t-1));
		PsgWrite(p&0xff,(t-1)*2);
		PsgWrite((p>>8),t*2-1);
		PsgWrite(v,7+t);
		PsgWrite(enable,7);
	} else
		PsgWrite(0,7+t);
}
soundoff()
{
	PsgWrite(0,8);
	PsgWrite(0,9);
	PsgWrite(0,10);
}
fruitsound()
{
	if(Silent)
	    return;
	PsgWrite(037,PSG_NPERIOD);
	PsgWrite(PSG_EENABLE,PSG_0VOLUME);
	PsgWrite(0,PSG_EPERIOD);
	PsgWrite(050,PSG_EPERIOD+1);
	PsgWrite(011,PSG_SHAPE);
	PsgWrite(067,PSG_ENABLE);
}
