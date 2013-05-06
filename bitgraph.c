/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File: bitgraph.c
 *	      Contents:	bitgraph dependent routines
 *	        Author: Bob Brown (rlb)
 *	   Description: Most of the terminal dependent features are
 *			in this source module and sounds.c
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

/*
 * Most of getscores() comes from Rich Fortier at BBN (rwf@BBN-UNIX)
 */

#include "bithdwr.h"
#include "bg.h"
#include "style.h"
#include "pacman.h"
#include "host.h"
#include "compat.h"

/*
 * terminit - set up the terminal - called once at beginning of a session.
 *
 * Save the current terminal context and set the screen to white-on-black.
 * Get the high scores from the host, if it offers them.
 */

terminit()
{
	dopush();
	DoRsetBG(5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#if V5_01 || V5_02
	KbdInit();
#endif
#ifdef notdef
	screenm = 0;
	setcontext();
#endif
}

/*
 * termwrapup - clean up terminal state - called once at end of session.
 *
 * Pops terminal context back off context stack.
 */

termwrapup()
{
#if V5_01 || V5_02
	KbdReset();
#endif
	black();
	dopop();
}
#ifdef HOSTCODE
/*
 * getscores - get the high score vector from the host.
 *
 *   Scores are transmitted as ESC : arg0;arg1;...CAN
 *   where CAN is the ANSI standard cancel sequence.
 *   The cancel ensures the BitGraph will do no processing
 *   if the downloaded program doesn't read the arg vector.
 *   This routine times out in two seconds in case the host
 *   doesn't send the scores.
 */

getscores ()
{
	register int  c;
	register int  i;
	long	*argp;
	extern short argc;
	extern long arg0, arg1;

	argc = arg0 = arg1 = 0;
	for (i = 0; i < 1000; i++) {
		snooze (2L);             /* Wait for max of 2 msec.  */
		if ((c = hstgetc ()) == '\033') {
			getchar ();         /* Throw away the colon.    */
			doargs  ();         /* Read the arg list.       */
			break;
		}
	}
	argp = &arg0;
	for ( i=0 ; i<MAXGAMES ; i++ )
		if ( i < argc )
			Highscore[i] = *argp++;
		else
			Highscore[i] = NOSCORE;
}
#endif
/*
 * settextop - set text operation mode
 *
 * Used to change text drawing mode to REPLACE (for score & wave) or to
 * XOR (for points on interior of game board).
 */

settextop(i)
int i;
{
	switch (i) {
	case INVERT:
		SetOp(RASTXOR);
		break;
	case REPLACE:
		SetOp(RASTRPL);
		break;
	default:
	        error("Bogus argument to settextop()");
	        break;
	}
}

/*
 * mvprintf - do a printf somewhere.
 */
/* VARARGS3 */
mvprintf(row,col,f,x1)
char *f;
unsigned x1;
{
	extern int showchar();

	if(col<0)
		col = 0;
	DoHvp (row, col);
	doprf(f,&x1,showchar);

}
black()
{
#if V5_01 || V5_02
        register long *cp;
	for(cp=(long *)DISPLAY;cp<(long *)EDISPLAY;cp++)
	        *cp = 0;
#else
	RastDisplay(RASTSET);
#endif
}

#if V5_01 || V5_02
/*
 * Keyboard routines for the version 5.01 terminals
 *
 * KbdInit - call ONCE at beginning.
 * KbdFlush - flushes queues.
 * KbdReset - call ONCE at end.
 * KbdGetc - call to get next character.
 */

#define GINFLAG ((char *)0x100032)
extern int acktoz80(), (*g_kbdgin)();
int (*ginsave)();
char Queue[100], *Qend, *Qhead, *Qtail;
int KbdInt();

KbdInit()
{
	ginsave = g_kbdgin;
	g_kbdgin = KbdInt;
	*GINFLAG = 1;
	KbdFlush();
}
KbdReset()
{
	g_kbdgin = ginsave;
	*GINFLAG = 0;
}
KbdFlush()
{
	Qhead = Qtail = Queue;
	Qend = &Queue[sizeof Queue];
}

KbdInt()
{
	char *next;
	int c;

	c = dummy();
	next = Qtail+1;
	if(next>=Qend)
		next = Queue;
	if (next!=Qhead) {
		*Qtail = c;
		Qtail = next;
	}
	g_kbdgin = KbdInt;
	*GINFLAG = 1;
	acktoz80();
}

dummy(){}

KbdGetc()
{
	int c;

	if(Qhead==Qtail)
		return -1;
	c = *Qhead++;
	c &= 0xff;
	if(Qhead>=Qend)
		Qhead = Queue;
	return c;
}
#endif
