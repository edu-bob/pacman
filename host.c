/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File:	hostscore.c
 *	      Contents:	routines for host score file support
 *	        Author: Rich Fortier (rwf)
 *			Bolt Beranek & Newman
 *		  Date: May, 1982
 *	   Description:	support for keeping a high score file on the
 *			host.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *
 *----------------------------------------------------------------------
 */
#ifdef HOSTCODE
#include "style.h"
#include "host.h"

/*****************************************************************************
* encrypt:  trivial encryption routine.                                      *
*                                                                            *
*   Arguments:  num, the number to encrypt.                                  *
*                                                                            *
*   Returns:    the encrypted number.                                        *
*                                                                            *
*   Remarks:   this  routine  is  in no way intended to provide any          *
* security in the numbers transmitted back to the host.  It's  sole          *
* purpose  is  to  make  is sufficiently tedious to type in a bogus          *
* number to the host that most people won't bother.                          *
*                                                                            *
*****************************************************************************/

long encrypt (num)
register long num;
{
    return num ^ 0xA5A5L;
}


/*****************************************************************************
* sndhost:  formatted print routine.                                         *
*                                                                            *
*   Arguments:  fmt, the print format.                                       *
*               x1,  the first argument.                                     *
*                                                                            *
*   Returns:    nothing.                                                     *
*                                                                            *
*   Remarks:  simply invoke lower level routine with proper display routine. *
*                                                                            *
*****************************************************************************/

/*VARARGS1*/
sndhost (fmt, x1)
char *fmt;
unsigned x1;
{
    extern int hstput ();

    doprf(fmt, &x1, hstput);
}




/*****************************************************************************
* hstput:  host put a character routine.                                     *
*                                                                            *
*   Arguments:  c, the character to put.                                     *
*                                                                            *
*   Returns:    nothing.                                                     *
*                                                                            *
*   Remarks:  simply invoke the queue put routine, looping until success.    *
*                                                                            *
*****************************************************************************/

hstput (c)
register int c;
{
    extern int hstputc();
    extern int hostpc();

#if V1_25
    while (hostpc (c) == -1);
#else
    while (hstputc (c) == -1);
#endif

}


#endif



