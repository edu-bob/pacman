/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File:	fruit.c
 *	      Contents:	fruit handling routines
 *	        Author: Bob Brown (rlb)
 *	   Description:	Fruits are put up for grabs at most twice per
 *			wave (hence stalling buys nothing).  They are
 *			scheduled to go off at a random time when the
 *			wave is started.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

#include "style.h"
#include "pacman.h"

/*
 * Scheduling:
 *	At the beginning of a wave or round, fruitinit is called to schedule
 *	the appearance of a fruit.  A typical wave lasts 80-90 scheduling
 *	seconds, hence the first fruit always appears sometime in the first
 *	half of the wave.
 *	Once "alive", the death of the fruit is scheduled.  If the pacman
 *	gets to the fruit first, the death routine does nothing when the
 *	time elapses except schedule a new fruit.
 */

fruitinit(reset)
bool reset;
{
	int fruiton();
	Fruit.alive = FALSE;
	Fruit.row = MZtoSC(FRUITROW);
	Fruit.col = MZtoSC(FRUITCOL);
	if ( reset )
		Fruit.thiswave = FRUITSPERWAVE;
	eladd(grand(MINFRUITDELAY,MAXFRUITDELAY),fruiton,NULL,0);
}

fruitcomp()
{
	if ( Fruit.alive )
		blt40(Fruit.font,Fruit.row,Fruit.col,INVERT);
}

/*
 * fruiton - turn on the fruit, start timer to turn it off
 */
int
fruiton()
{
	int fruitoff();
	Fruit.alive = TRUE;
	Fruit.thiswave--;
	Fruit.value = (Wave+1)/2 * VALFRUIT;
	Fruit.font = FRUITCHAR /* + max(Wave/2,MAXFRUITFONT) */ ;
	fruitcomp();
	eladd(FRUITTIME,fruitoff,NULL,0);
}

int
fruitoff()
{
	int fruiton();
	if ( Fruit.alive ) {
		fruitcomp();
		Fruit.alive = FALSE;
	}
	if ( Fruit.thiswave > 0 )
		eladd(grand(MINFRUITDELAY,MAXFRUITDELAY),fruiton,NULL,0);

}

hasfruit()
{
	if ( Fruit.alive )
		return (iabs(Fruit.row-Pacman.row)<3 && iabs(Fruit.col-Pacman.col)<3 );
	return FALSE;
}

hitfruit()
{
	int crow, ccol;
	addscore(Fruit.value);
	settextop(INVERT);
	crow = SCtoROW(Fruit.row)+1;
	ccol = SCtoCOL(Fruit.col)-6;
#ifdef GAUDY
	explode(Fruit.row, Fruit.col);
#endif
	mvprintf(crow, ccol, "%4d",Fruit.value);
#if V1_25 | V1_76 | V2_0
	sleep(750);
#else
	snooze(750L);
#endif
	mvprintf(crow, ccol, "%4d",Fruit.value);
	settextop(REPLACE);
	fruitcomp();
	Fruit.alive = FALSE;
}
