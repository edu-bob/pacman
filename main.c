/*----------------------------------------------------------------------
 *
 *	PACMAN for BBN BitGraphs
 *
 *	          File: main.c
 *	      Contents:	controlling and utility routines
 *	        Author: Bob Brown (rlb)	
 *	   Description: routines for setting up and controlling the
 *			game, plus miscellaneous utilities.
 *
 * Copyright (c) 1987 Robert L. Brown. All Rights Reserved. This code
 * is public domain.
 *
 *
 *---------------------------------------------------------------------
 */

#include "bithdwr.h"
#include "style.h"
#include "pacman.h"
#include "host.h"
#include "compat.h"

/*
 * Implementation and Game notes
 *
 *    o	The program is driven by a passive scheduler.  The main loop
 *	consists of switching back and forth between checking for keyboard
 *	input and some event becoming ready.
 *    o Objects are drawn on the screen using exclusive-ors.  Maze pieces are
 *	24x24 bits...others are 40x40 bits.
 *    o Code was written as an exercise in developing non-trivial programs for
 *	the BBN BitGraph.  All code was written by the Bob Brown except the
 *	host-communications code, which was provided by	Rich Fortier at BBN.
 *    o Several tricks and features are only documented in the code.  It takes
 *	an understanding of these to reach 100,000 in game four...unless you're
 *	very very clever.
 *    o If you have a score for the music played in the arcade version, send
 *	it to me (rlb@purdue, ...!decvax!pur-ee!purdue!rlb) and I'll put it
 *	in.
 *    o BBN has explicitly asked that I do not distribute the symbol table for
 *	the resident control program...contact your BBN sales rep.
 *    o Watch info-bitgraph@mit-mc for updates.  Send enhancements to me.
 *	Since I'm no pacmanophile (I had played it a handful of times before
 *	writing this), if you know of tricks or quirks in the arcade version
 *	that I may have missed, let me know about them.  I don't pretend to
 *	emulate the arcade version exactly, just want a reasonable facsimile.
 */

/*
 * Instances of global variables not defined elsewhere
 */

bool	Abort;			/* TRUE => 'q' typed			*/
char	*Board[];		/* The game board			*/
int	Dotsrem;		/* Dots remaining to gobble up		*/
int	Game;			/* Selected games number		*/
long	Highscore[MAXGAMES];	/* High score per game from host	*/
int	MaxWave[MAXGAMES];	/* Wave on which high score was		*/
long	Lastscore;		/* score of last game			*/
int	Monbase;		/* Base scheduler delay for monsters	*/
bool	Monseeking;		/* TRUE=>they know where he is		*/
int	Monvalue;		/* value of captured monster		*/
int	Newgame;		/* 1-4 for new game number		*/ 
long	Nextbonus;		/* Where the next bonus pacman comes	*/
int	Pacmen;			/* Number of Pacman remaining		*/
bool	Paused;			/* TRUE => game is paused		*/
bool	Running;		/* TRUE => no reason to stop running	*/
long	Score;			/* Current score, current game		*/
bool	Silent;			/* TRUE => make no noise		*/
bool	Wallsoff;		/* TRUE => interior walls are off	*/
int	Wave;			/* Current wave #, current game		*/

#ifdef BILL
int	BillMode;
#endif
/*
 *  Game parameters
 */

int	Seekprob;		/* Base intelligence of Monsters	*/
int	Seekinc;		/* They get this smarter every 2 waves	*/

/*
 * main - game logic control.
 *
 *  contortions are because there's no exit().
 */

main()
{
	int i;
#ifdef BILL
	BillMode = 1;
#endif
	terminit();
	for(i=0;i<MAXGAMES;i++) {
		Highscore[i] = 0;
		MaxWave[i] = 0;
	}

	Lastscore = -1;
#ifdef HOSTCODE
	getscores();
#endif
	VolumeControl = 8;
	Abort = FALSE;
	Silent = FALSE;
	Running = TRUE;
	Paused = FALSE;
	Newgame = 0;
	/*
	 * For each game played...
	 */
	while ( Running ) {
		/*
		 * For each new wave...
		 */
		newgame();
		while ( Pacmen > 0 && Running ) {
			newwave();
			/*
			 * For each game scan...
			 */
			while ( Dotsrem > 0 ) {
				/*
				 * If the Pacman died, start a new one.
				 */
				if ( !Pacman.alive ) {
					addmen(-1);
					if ( Pacmen <= 0 )
						break;
					elinit();
					if ( Wallsoff ) {
						compwalls();
						Wallsoff = FALSE;
					}
					pacinit();
					moninit();
					fruitinit(FALSE);
				}
				do {
					kbpoll();
				} while ( Paused && Newgame==0);
				if ( Abort || Newgame )
					break;
				elpoll();
			}
			soundoff();
			if ( Abort || Newgame )
				break;
			paccomp();
			moncomp();
			fruitcomp();
			soundoff();
		}
		if (Game != 0) {
#ifdef HOSTCODE
			sndhost ("%s%s%d\n%s%D %D\n", SYNCSTRING, 
            		       GAMESTRING,  Game, 
	        	       SCORESTRING, (long)Score,
			       (long)KEY);
#endif
			Lastscore = Score;
			if ( Score > Highscore[Game-1] ) {
				Highscore[Game-1] = Score;
				MaxWave[Game-1] = Wave;
			}
		}
		if ( Abort )
			break;
	}
#ifdef HOSTCODE
	sndhost ("%s", EXITSTRING);
#endif
	termwrapup();
}


/*
 * newgame - set up for a new game, with instructions.
 */

newgame()
{
	if (Newgame <= 0) {
		instruct();
		Game = 0;
	/*	while(Getc()!=-1) ; /* flush keyboard queue */
		while (Game==0 && Running) kbpoll();
	} else
		Game = Newgame;
	if ( !Running )
		return;
	black();

	switch ( Game ) {
	case 1:
		Seekprob = 40;
		Seekinc = 10;
		Monbase = 40;
		Pacman.time = 43;
		break;
	case 2:
		Seekprob = 80;
		Seekinc = 2;
		Monbase = 30;
		Pacman.time = 35;
		break;
	case 3:
		Seekprob = 100;
		Seekinc = 0;
		Monbase = MINMONTIME;
		Pacman.time = MINPACTIME;
		break;
	case 4:
		Seekprob = 90;
		Seekinc = 0;
		Monbase = 0;
		Pacman.time = 0;
	}
	Newgame = 0;
	Pacman.alive = TRUE;
	Wave = 0;
	Score = 0;
	Nextbonus = SCOREMOD;
	settextop(REPLACE);
	mvprintf(SCOREROW,1,"S C O R E : ");
	addscore(0);
	addscore(FLUSHSCORE);
#ifdef HIGHSCORE
	if ( Highscore[Game-1] != NOSCORE )
		mvprintf(SCOREROW,HIGHCOL,"H I G H   S C O R E : %D",(long)Highscore[Game-1]);
#endif
	Pacmen = 1;
	mvprintf(WAVEROW,WAVECOL,"W A V E : ");
	addmen(PACMEN-1);
	pacnew();
	monnew();
}
/*
 * Display Game instructions
 */
instruct()
{
	int i;

	black();
	settextop(REPLACE);
	mvprintf(11,1,"P A C M A N");
	mvprintf(13,10,"<h> or <keypad-4> to move left.");
	mvprintf(14,10,"<j> or <keypad-2> to move down.");
	mvprintf(15,10,"<k> or <keypad-8> to move up.");
	mvprintf(16,10,"<l> or <keypad-6> to move right.");
	mvprintf(17,10,"< > to stop.");

	mvprintf(19,10,"<q> to quit.");
	mvprintf(20,10,"<n> to start a new game.");
	mvprintf(21,10,"<s> to toggle sound.");
	mvprintf(23,10,"Select game 1, 2, 3, or 4");
#ifdef BILL
	if(BillMode)
		printf(".");
#else
	printf(".");
#endif
	mvprintf(26, 10, "GAME    HIGH SCORE    WAVE");
	for(i=0;i<MAXGAMES;i++)
		mvprintf(27+i, 10, "%3d      %7d      %3d", i+1, Highscore[i], MaxWave[i]);
	if(Lastscore>=0)
		mvprintf(27+i+1, 10, "Last Score: %d", Lastscore);
	return;
}
/*
 * start a new wave - also starts the first wave.
 *
 *  increase the speed on the monsters, but not too fast.
 *  However, if already running faster than fastest, leave it there.
 */

newwave()
{
	register monster *mptr;
	addscore(FLUSHSCORE);
	Wave++;
	mvprintf(WAVEROW,WAVECOL+10,"%D",Wave);
	if ( Wave&1 ) {
		for ( mptr=Monster ; mptr < &Monster[MAXMONSTER] ; mptr++ )
			if ( mptr->time > MINMONTIME )
				mptr->time = max(mptr->time-MONDELTA,MINMONTIME);
		if ( Pacman.time > MINPACTIME )
			Pacman.time = max(Pacman.time-PACDELTA,MINPACTIME);
	}
	newboard();
	drawboard();
	Dotsrem = DOTS;
	Seekprob += Seekinc;
	elinit();
	pacinit();
	moninit();
	fruitinit(TRUE);
}
/*
 * Keyboard polling routine
 */
kbpoll()
{
	int c;
	rsetdead();
	c = Getc();
	if (c == -1)
	        return;
	switch (c&0xff) {
#ifdef BILL
	case 'B':
		BillMode = !BillMode;
		break;
#endif
	case '+':
	        addmen(1);
	        break;
	case 'P':
#ifdef NOSCROLL
	case NOSCROLL:
#endif
		Paused = !Paused;
		break;
	case 'w':
		compwalls();
/*		Wallsoff = !Wallsoff; */
		break;
	case GAME4:
	case GAME3:
	case GAME2:
	case GAME1:
		if ( Game ) {
			Newgame = c-'0';
			break;
		}
		Game = c - '0';
		break;
	case LEFT:
	case ALTLEFT:
		Paused = FALSE;
		Pacman.pending = MOVELEFT;
		Pacman.pendcnt = PENDCOUNT;
		Pacman.moving = TRUE;
		break;
	case RIGHT:
	case ALTRIGHT:
		Paused = FALSE;
		Pacman.pending = MOVERIGHT;
		Pacman.pendcnt = PENDCOUNT;
		Pacman.moving = TRUE;
		break;
	case UP:
	case ALTUP:
		Paused = FALSE;
		Pacman.pending = MOVEUP;
		Pacman.pendcnt = PENDCOUNT;
		Pacman.moving = TRUE;
		break;
	case DOWN:
	case ALTDOWN:
	case ALTALTDOWN:
		Paused = FALSE;
		Pacman.pending = MOVEDOWN;
		Pacman.pendcnt = PENDCOUNT;
		Pacman.moving = TRUE;
		break;
	case SILENT:
		Silent = !Silent;
		break;
	case STOP:
		Paused = FALSE;
		Pacman.moving = FALSE;
		Pacman.pendcnt = 0;
		break;
	case ABORT:
		Running = FALSE;
		Abort = TRUE;
		break;
	case NEWGAME:
		Newgame = -1;
		break;
	case LOUDER:
		if (VolumeControl < 16)
		    VolumeControl++;
		break;
	case SOFTER:
		if (VolumeControl > 1)
		    VolumeControl--;
		break;
	}
}

/*
 * Update the score
 *
 * Adds 'inc' to the score and updates the screen if the score crosses a zero
 * mod 100 value (for efficiency).  If 'inc' is FLUSHSCORE, then the current
 * score is updated on the screen.
 *
 * Also adds in a bonus man if so earned.  Bonus men are given at 10k, 20k,
 * 40k, 80k, 160k, etc.
 */

addscore(inc)
register int inc;
{
	register long  oldscore;
	settextop(REPLACE);
	if ( inc == FLUSHSCORE ) {
		mvprintf(SCOREROW,SCORECOL,"%D",(long)Score);
		return;
	}
	oldscore = Score;
	Score += (long)inc;
	if ( Score/100 != oldscore/100 ) {
		mvprintf(SCOREROW,SCORECOL,"%D",(long)Score);
	}
	if ( oldscore<Nextbonus &&  Score>=Nextbonus ) {
		addmen(1);
/*		Nextbonus *= 2;*/ /* Bill says it's too hard */
		Nextbonus += SCOREMOD;
	}
}
/*
 * Add multiple men or subtract just one.
 */
addmen(inc)
register int inc;
{
	register int i;
	/*
	 * Put another at the bottom of the screen
	 */
	if ( inc > 0 ) {
		for ( i=0 ; i<inc ; i++ ) {
			rempacface();
			Pacmen++;
		}
	} else {
		Pacmen--;
		rempacface();
	}
}
#define PRIME 2131
grand(lb,ub)
int lb, ub;
{
	return (rand() % PRIME) * (ub-lb+1) / PRIME + lb;
}
long	randx = 1;

srand(x)
unsigned x;
{
	randx = x;
}

rand()
{
	return(((randx = randx*1103515245 + 12345)>>16) & 077777);
}

pause(ms)
{
	soundoff();
#if V1_25 | V1_76 | V2_0 
	sleep(ms);
#else
	snooze((long)(ms));
#endif
	eladjust(ms);
}

debug (args)
char *args;
{
    DoHvp (1, 1);
    printf ("%r", &args);
    clreol (0);
    snooze (1000L);
    rsetdead ();
}

