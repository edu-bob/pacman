/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File:	sched.c
 *	      Contents:	passive real-time-clock-based scheduler
 *	        Author: Bob Brown (rlb)
 *	   Description:	An event list manager for the scheduling
 *			of C procedures after given time delays
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

#include "style.h"

/*
 * Beacuse the scheduler calls routines that re-enqueue themselves,
 * The following constant is needed to prevent elpoll() from starving
 * the keyboard polling routine when the monsters are running flat-out.
 */

#define MAXLOOPS 5

/*
 * elinit() - initialize the event list to empty.
 * eladd() - add an event to the event list.
 * elpoll() - called at in polling loop to run scheduled events.
 */

/*
 *  MAXEVENTS ...
 *
 *	1 for pacman
 *	4 for monsters
 *	4 for power pill effects
 *	1 for bonus fruits
 *	3 for tone generators (probably need many more)
 *
 */

#define MAXEVENTS	24
#define NULLEVENT       (event *)(0)

typedef struct ev {
	struct ev *next;
	long time;
	int (*routine)();	/* procedure to call */
	char *arg0;		/* optional first argument */
	int arg1;		/* optional second argument */
} event;

event *Eventfree;
event *Eventhead;
event Event[MAXEVENTS];

extern long clock;

elinit()
{
	register event *ep;
	Eventfree = NULLEVENT;
	for ( ep=Event ; ep<&Event[MAXEVENTS] ; ep++ ) {
		ep->next = Eventfree;
		Eventfree = ep;
	}
	Eventhead = NULLEVENT;
	clock = 0;
}
/*
 * eladd - add an event to the event list
 */
/* VARARGS 2 */
eladd(delta,routine,arg0,arg1)
int delta;
int (*routine)();
char *arg0;
int arg1;
{
	long time;
	register event *ep, *pp, *cp;

#if V1_25
	delta = (delta+5)/10; /* 100HZ clock */
#endif

	if ( Eventfree == NULLEVENT ) {
		error("Event list overflow");
		return;
	}
	cp = Eventfree;
	Eventfree = Eventfree->next;

	time = delta+clock;
	cp->time = time;
	cp->routine = routine;
	cp->arg0 = arg0;
	cp->arg1 = arg1;

	ep = Eventhead;
	pp = NULLEVENT;
	while ( ep!=NULLEVENT && time>ep->time ) {
		pp = ep;
		ep = ep->next;
	}
	cp->next = ep;
	if ( pp == NULLEVENT )
		Eventhead = cp;
	else
		pp->next = cp;
}
/*
 * elpoll - check if anything to schedule
 */
elpoll()
{
	register int maxloops;
	register event *ep;
	maxloops = MAXLOOPS;
	while ( (ep=Eventhead)!=NULLEVENT && ep->time < clock && --maxloops > 0) {
		Eventhead = ep->next;
		(*ep->routine)(ep->arg0,ep->arg1);
		ep->next = Eventfree;
		Eventfree = ep;
	}
}
/*
 * eladjust - adjust the scheduler based on processor delay
 */
eladjust(ms)
register int ms;
{
#if V1_25
	ms = (ms+5)/10;
#endif
	clock -= ms;
}
