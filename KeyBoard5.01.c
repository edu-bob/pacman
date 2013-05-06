/*
 * Keyboard routines for the version 5.01 terminals
 *
 * KbdInit - call ONCE at beginning.
 * KbdFlush - flushes queues.
 * KbdReset - call ONCE at end.
 * KbdGetc - call to get next character.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
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
	if(Qhead>=Qend)
		Qhead = Queue;
	return (int)c;
}
