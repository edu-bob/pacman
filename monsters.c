/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File: monsters.c
 *	      Contents:	monster definitions and manipulation routines
 *	        Author: Bob Brown (rlb)
 *	   Description:	Initialization, movement, drawing routines.
 *			Interacts with several pacman routines.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *----------------------------------------------------------------------
 */

#include "style.h"
#include "pacman.h"
#ifdef BILL
extern int BillMode;
#endif
/*
 *
 *  If ARCADE is defined, then monsters can see through the walls when
 *  hunting the pacman.
 *
 *  Monster structure initialization
 *
 * The following lists the static part of the monster structures, in the order
 * that they leave the stable.
 */

monster Monster[MAXMONSTER] = {
	{ 45, 36, MOVEUP,     0,    5,   -10 },
	{ 45, 41, MOVEUP,    24,    0,   -20 },	/* fast, but stupid */
	{ 39, 33, MOVEDOWN,  52,    9,     0 },
	{ 39, 48, MOVEDOWN,  80,   13,     5 },	/* slow, but methodical */
};

/*
 * monnew - create a new set of monsters.
 *
 *	Called once at the beginning of a game.
 */

monnew()
{
	register monster *mptr;
	for ( mptr=Monster ; mptr < &Monster[MAXMONSTER] ; mptr++ )
		mptr->time = Monbase + mptr->inittime;
}

/*
 * moninit - reinitialize the monsters.
 *
 *	Called at the beginning of each wave or when Pacman dies.
 */

moninit()
{
	register monster *mptr;
	for ( mptr=Monster ; mptr < &Monster[MAXMONSTER] ; mptr++ ) {
		if ( mptr == Monster ) {
			mptr->row = STARTROW;
			mptr->col = STARTCOL;
			mptr->dir = MOVERIGHT;
			mptr->movestate = ROAMING;
		} else {
			mptr->row = mptr->initrow;
			mptr->col = mptr->initcol;
			mptr->dir = mptr->initdir;
			mptr->movestate = STABLED;
		}
		mptr->mrow = SCtoMZ(mptr->row);
		mptr->mcol = SCtoMZ(mptr->col);
		mptr->state = DANGEROUS;
		mptr->delay = mptr->initdelay;
		mptr->font = MONSTERCHAR[0];
		Monseeking = FALSE;
		mondraw(mptr);
		eladd(mptr->time+STARTDELAY,monmove,(char *)mptr, 0);
	}
}
/*
 * monfont - change the font of the monsters.
 *
 *	Called when Pacman swallows a pill or starts getting weaker.
 */

monfont(chr)
char chr;
{
	register monster *mptr;
	for ( mptr=Monster ; mptr < &Monster[MAXMONSTER] ; mptr++ ) {
		monfont1(mptr,chr);
	}
}

monfont1(mptr,chr)
register monster *mptr;
char chr;
{
	mondraw(mptr);
	mptr->font = chr;
	mondraw(mptr);
}
/*
 * moncomp - turn on/off (i.e. complement) all the monsters
 */

moncomp()
{
	register monster *mptr;
	for ( mptr=Monster ; mptr < &Monster[MAXMONSTER] ; mptr++ )
		mondraw(mptr);
}

/*
 * mondraw - complement a single monster
 */

mondraw(mptr)
register monster *mptr;
{
	blt40(mptr->font,mptr->row,mptr->col,INVERT);
}

/*
 * Move a particular monster
 *
 * This routine moves a given monster based its current movestate
 */

int
monmove(mptr)
register monster *mptr;
{
	register move *newmove;
	int newdelay;

	/*
	 * First, turn off the monster (mondraw uses current state, which is
	 * changed here).
	 *
	 * Motion depends on the monster movestate
	 */

	mondraw(mptr);
#ifdef BILL
	if(BillMode&&Wave>10 && grand(0, 100)/2 == 27)
		mptr->canreverse = TRUE;
#endif
	switch ( mptr->movestate ) {

	/*
	 * In the center box - won't come out until time delay elapses.
	 * (should check for pending ambush here).
	 */

	case STABLED:
		newmove = bounce(mptr);
		if ( mptr->delay > 0 )
			mptr->delay = max(mptr->delay-1,0);
		else
			mptr->movestate = LEAVING;
		break;
	case LEAVING:
		newmove = tostart(mptr);
		if ( newmove->row == STARTROW && newmove->col == STARTCOL )
			mptr->movestate = ROAMING;
		break;

	/*
	 * If roaming about, switch to seeking if someone sees pacman then fall
	 * into seeking code.
	 */

	case ROAMING:
		if ( Monseeking )
			mptr->movestate = SEEKING;
		else if ( seeshim(mptr) ) {
			Monseeking = TRUE;
			mptr->movestate = SEEKING;
		} else {
			newmove = dumbmove(mptr);
			break;
		}

	/*
	 *  When seeking the pacman, they may make a random move.  If the
	 *  Pacman is powerful, then they move away if edible, else random.
	 */

	case SEEKING:
		if ( grand(0,100) < Seekprob+mptr->bonus && !Pacman.safe && !(Pacman.power>0 && mptr->state==DANGEROUS))
			newmove = seekmove(mptr);
		else
			newmove = dumbmove(mptr);
		break;

	/*
	 * Homing means the monster has been eaten and it's going home.
	 */

	case HOMING:
		newmove = todoor(mptr);
		if ( newmove->row == STARTROW && newmove->col == STARTCOL )
			mptr->movestate = HOMING2;
		break;
	case HOMING2:
		newmove = tostable(mptr);
		if ( newmove->row==mptr->initrow && newmove->col==mptr->initcol ) {
			mptr->movestate = STABLED;
			mptr->state = DANGEROUS;
			mptr->font = MONSTERCHAR[mptr->initdir];
			newmove->dir = mptr->initdir;
			mptr->delay = Pacman.power+(mptr-Monster)*24;
		}
		break;
	}
	/*
	 * Move the monster on the screen
	 */
	mptr->row = newmove->row;
	mptr->col = newmove->col;
	mptr->dir = newmove->dir;
	mptr->mrow = SCtoMZ(mptr->row);	/* efficiency hack */
	mptr->mcol = SCtoMZ(mptr->col);
	if ( mptr->state == DANGEROUS )
		mptr->font = MONSTERCHAR[mptr->dir];
	mondraw(mptr);

	/*
	 * Check for collision with the pacman
	 */

	if ( mptr->state == DANGEROUS && collide(mptr) && Pacman.alive ) {
		pacdie();
		return;
	}

	/*
	 *  The time till next move depends on whether the monster is alive,
	 *  or is in the slow corridor, or is weakened by the pacman's power.
	 */

	newdelay = mptr->state==DEAD ?
#ifndef BILL
		EYESDELAY
#else
		(BillMode && Wave>10 && grand(0, 100)<25 ? EYESDELAY/4 : EYESDELAY)
#endif
		: inslow(mptr->row,mptr->col) ? min(Pacman.time,mptr->time)+SLOWDELTA:
		mptr->time+Pacman.power*POWERDELAY;

	eladd(newdelay,monmove,mptr, 0);
}

/*
 * Simplest of all possible move commands - chooses random direction
 */

move *
dumbmove(mptr)
register monster *mptr;
{
	register int moves;
	static move movelist[4];
	if ( !aligns(mptr->row, mptr->col) ) {
		onestep(mptr->row,mptr->col,mptr->dir,&movelist[0]);
		return movelist;
	}
	moves=compmoves(mptr->row,mptr->col,movelist,mptr->dir,FALSE);
	if (moves == 1 )
		return movelist;
	return &movelist[grand(0,moves-1)];
}

/*
 *  Direction seeking Monster move
 */

move *
seekmove(mptr)
register monster *mptr;
{
	register int moves;
	static move movelist[4];
	if ( !aligns(mptr->row, mptr->col) ) {
		onestep(mptr->row,mptr->col,mptr->dir,&movelist[0]);
		return movelist;
	}

	/*
	 * Monsters can reverse once when the pacman swallows a pill -
	 * Should save it until the pacman is close because now they
	 * tend to turn right into the pacman's jaws (unimplemented).
	 */

	if ( mptr->canreverse ) {
		moves=compmoves(mptr->row,mptr->col,movelist,0,FALSE);
		mptr->canreverse = FALSE;
	} else
		moves=compmoves(mptr->row,mptr->col,movelist,mptr->dir,FALSE);
	if (moves == 1 )
		return movelist;
	if ( mptr->state == EDIBLE )
		return furthest(movelist,moves,Pacman.row,Pacman.col);
	return closest(movelist,moves,Pacman.row,Pacman.col);
}

/*
 *  Compute all the possible moves a monster can make.
 *
 */

compmoves(mrow, mcol, list, curdir, doorok)
int mrow, mcol, curdir;
register move *list;
bool doorok;
{
	register int moves, i;
	/*
	 *  The list of moves will not include the direction opposite
	 *  of curdir, if it is nonzero.  This statement computes the
	 *  opposite direction effeciently.
	 */
	if ( curdir > 0 )
		curdir = ((curdir+1)&3)+1;
	moves = 0;
	for ( i=1 ; i<=4 ; i++ ) {
		if ( i == curdir )
			continue;
		onestep(mrow,mcol,i,list);
		if ( (doorok && hitdoor(list->row,list->col)) ||
		    !(hitwall(list->row,list->col,i)||(i==MOVEUP&&hitblk(list->row,list->col))) ) {
			list++;
			moves++;
		}
	}
	return moves;
}
onestep(mrow, mcol, dir, list)
register int mrow, mcol, dir;
register move *list;
{
	list->col = mcol;
	list->row = mrow;
	list->dir = dir;
	switch ( dir ) {
	case MOVERIGHT:
		list->col++;
		break;
	case MOVEUP:
		list->row--;
		break;
	case MOVELEFT:
		list->col--;
		break;
	case MOVEDOWN:
		list->row++;
		break;
	}
	if ( list->col < 0 )
		list->col += MZtoSC(MAZECOLS);
	if ( list->col >= MZtoSC(MAZECOLS) )
		list->col = 0;
}
/*
 * Choose the move from a list the goes in the direction closest to a
 * given point.
 *
 *   SCREEN COORDINATES
 */
move *
closest(list,moves,row,col)
register move *list;
register int moves;
register int row, col;
{
	register move *lptr;
	int dist, diff, diffr, diffc;
	dist = 0x7fff;
	for ( ; moves>0 ; moves-- ) {
		diffr = iabs(row-list->row);
		diffc = iabs(col-list->col);
		diff = max(diffr,diffc);
		if ( diff < dist ) {
			lptr = list;
			dist = diff;
		}
		list++;
	}
	return lptr;
}
move *
furthest(list,moves,row,col)
register move *list;
register int moves;
register int row, col;
{
	register move *lptr;
	int dist, diff, diffr, diffc;
	dist = -1;
	for ( ; moves>0 ; moves-- ) {
		diffr = iabs(row-list->row);
		diffc = iabs(col-list->col);
		diff = min(diffr,diffc); /* min causes lots of turns */
		if ( diff > dist ) {
			lptr = list;
			dist = diff;
		}
		list++;
	}
	return lptr;
}

/*
 * Check if a monster "sees" the pacman
 */

seeshim(mptr)
register monster *mptr;
{
	register int rdiff, cdiff;
	register int i;
	rdiff = iabs(mptr->row - Pacman.row);
	cdiff = iabs(mptr->col - Pacman.col);
	switch ( mptr->dir ) {
	case MOVERIGHT:
		if ( rdiff > 2 || mptr->col > Pacman.col )
			return FALSE;
#ifndef ARCADE
		for ( i=mptr->mcol ; i<Pacman.mcol ; i++ )
			if ( (Board[mptr->mrow][i]&TYPEMASK) <= WALLMAX )
				return FALSE;
#endif
		return TRUE;
	case MOVEUP:
		if ( cdiff > 2 || mptr->row < Pacman.row )
			return FALSE;
#ifndef ARCADE
		for ( i=mptr->mrow ; i>Pacman.mrow ; i-- )
			if ( (Board[i][mptr->mcol]&TYPEMASK) <= WALLMAX )
				return FALSE;
#endif
		return TRUE;
	case MOVELEFT:
		if ( rdiff > 2 || mptr->col < Pacman.col )
			return FALSE;
#ifndef ARCADE
		for ( i=mptr->mcol ; i>Pacman.mcol ; i-- )
			if ( (Board[mptr->mrow][i]&TYPEMASK) <= WALLMAX )
				return FALSE;
#endif
		return TRUE;
	case MOVEDOWN:
		if ( cdiff > 2 || mptr->row > Pacman.row )
			return FALSE;
#ifndef ARCADE
		for ( i=mptr->mrow ; i<Pacman.mrow ; i++ )
			if ( (Board[i][mptr->mcol]&TYPEMASK) <= WALLMAX )
				return FALSE;
#endif
		return TRUE;
	}
}

/*
 * Bounce the monster around in the stable
 */

move *
bounce(mptr)
register monster *mptr;
{
	static move newmove;
	onestep(mptr->row, mptr->col, mptr->dir, &newmove);
	if ( hitwall(newmove.row, newmove.col,mptr->dir) ) {
		mptr->dir = ((mptr->dir+1)&3)+1;
		onestep(mptr->row, mptr->col, mptr->dir, &newmove);
	}
	return &newmove;
}

/*
 * move a monster closer to the starting position
 */

move *
tostart(mptr)
register monster *mptr;
{
	static move movelist[4];
	int moves;
	moves = compmoves(mptr->row, mptr->col, movelist, mptr->dir, TRUE);
	if ( mptr->col != STARTCOL )
		return closest(movelist,moves, mptr->row, STARTCOL);
	return closest(movelist,moves, STARTROW, STARTCOL);
}

/*
 * Move the monster towards the door
 */

move *
todoor(mptr)
register monster *mptr;
{
	static move movelist[4];
	int moves;
	if ( !aligns(mptr->row, mptr->col) ) {
		onestep(mptr->row,mptr->col,mptr->dir,&movelist[0]);
		return movelist;
	}
	moves = compmoves(mptr->row, mptr->col, movelist, mptr->dir, FALSE);
	return closest(movelist,moves, STARTROW, STARTCOL);

}

/*
 * move a monster back to the stable
 */

move *
tostable(mptr)
register monster *mptr;
{
	static move movelist[4];
	int moves;
	moves = compmoves(mptr->row, mptr->col, movelist, mptr->dir, TRUE);
	if ( mptr->row!=mptr->initrow )
		return closest(movelist,moves, mptr->initrow, mptr->col);
	return closest(movelist,moves, mptr->initrow, mptr->initcol);
}
