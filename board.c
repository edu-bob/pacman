/*
 *----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraph
 *
 *	          File:	board.c
 *	      Contents:	declaration of game board and operations
 *	        Author: Bob Brown (rlb)
 *	   Description:	The Game board is not easily changed.  The
 *			characters used are also defined in pacman.h
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

#include "style.h"
#include "pacman.h"

/*
 * Due to memory limitations, the static game board shown below is
 * overwritten by the dynamic version.  There just isn't room for
 * two copies in the std. BitGraph.
 *
 * Only the low order four bits and the 040 bit are significant - the
 * others are "dynamic" and are set and reset during the course of a
 * game.  Of course, this depends on being ASCII.
 */

char *Board[] = {
	"B@@@@@@@@@@@@CB@@@@@@@@@@@@C",
	"AGGGGGGGGGGGGAAGGGGGGGGGGGGA",
	"AGb``cGb```cGAAGb```cGb``cGA",
	"AHaIIaGaIIIaGAAGaIIIaGaIIaHA",
	"AGe``dGe```dGEDGe```dGe``dGA",
	"AGGGGGGGGGGGGGGGGGGGGGGGGGGA",
	"AGb``cGbcGb``````cGbcGb``cGA",
	"AGe``dGaaGe``cb``dGaaGe``dGA",
	"AGGGGGGaaGGGGaaGGGGaaGGGGGGA",
	"E@@@@CGae``cIaaIb``daGB@@@@D",
	"IIIIIAGab``dMedMe``caGAIIIII",
	"IIIIIAGaaKKKKKKKKKKaaGAIIIII",
	"IIIIIAGaaKB@FFFF@CKaaGAIIIII",
	"@@@@@DGedKAIIIIIIAKedGE@@@@@",
	"JJJJJJGIIKAIIIIIIAKIIGJJJJJJ",
	"@@@@@CGbcKAIIIIIIAKbcGB@@@@@",
	"IIIIIAGaaKE@@@@@@DKaaGAIIIII",
	"IIIIIAGaaKKKKKKKKKKaaGAIIIII",
	"IIIIIAGaaIb``````cIaaGAIIIII",
	"B@@@@DGedIe``cb``dIedGE@@@@C",
	"AGGGGGGGGGGGGaaGGGGGGGGGGGGA",
	"AGb``cGb```cGaaGb```cGb``cGA",
	"AGe`caGe```dLedLe```dGab`dGA",
	"AHGGaaGGGGGGGIIGGGGGGGaaGGHA",
	"E@CGaaGbcGb``````cGbcGaaGB@D",
	"B@DGedGaaGe``cb``dGaaGedGE@C",
	"AGGGGGGaaGGGGaaGGGGaaGGGGGGA",
	"AGb````de``cGaaGb``de````cGA",
	"AGe````````dGedGe````````dGA",
	"AGGGGGGGGGGGGGGGGGGGGGGGGGGA",
	"E@@@@@@@@@@@@@@@@@@@@@@@@@@D",

};

/*
 * Generate a new board from the fixed board
 */
newboard()
{
	register int i,j;
	register char c;
	for ( i=0 ; i<MAZEROWS ; i++ )
		for ( j=0 ; j<MAZECOLS ; j++ ) {
			Board[i][j] &= (TYPEMASK|DISAPPEAR);
			switch ( Board[i][j] ) {
			case FIXPILL:
				Board[i][j] |= PILL;
				break;
			case BLKDOT:
			case FIXGOLD:
				Board[i][j] |= GOLD;
			}
		}
}
char chrmap[] = {"abcdefghi   h "};
drawboard()
{
	register int i,j;
	register char c;
	for ( i=0 ; i<MAZEROWS ; i++ ) {
		for ( j=0 ; j<MAZECOLS ; j++ ) {
			c = chrmap[Board[i][j] & TYPEMASK];
			if ( c!=' ' )
				blt24(c,MZtoSC(i),MZtoSC(j),REPLACE);
		}
		rsetdead();
	}
	Wallsoff = FALSE;
}

/*
 * Complement interior walls
 */

compwalls()
{
	register int i;
	register char c, *cp;
	rsetdead();
	for ( i=0 ; i<MAZEROWS ; i++ ) {
		for ( cp=Board[i] ; cp < &Board[i][MAZECOLS] ; cp++ ) {
			c = chrmap[*cp & TYPEMASK];
			if ( c!=' ' && (*cp & DISAPPEAR) )
				blt24(c,MZtoSC(i),MZtoSC(cp-Board[i]),INVERT);
			
		}
	}
}
