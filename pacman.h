/*----------------------------------------------------------------------*/
/*									*/
/*	PACMAN for BBN BitGraphs					*/
/*									*/
/*	          File: pacman.h					*/
/*	      Contents:	Constants, types, and global variables		*/
/*	        Author: Bob Brown (rlb)					*/
/*			Purdue CS					*/
/*		  Date: May, 1982					*/
/*	   Description: Manifest constants, some alterable, others	*/
/*			not easily changed.				*/
/*			Type names are short and lower case.		*/
/*			Global variables are all capitalized.		*/
/*			Constants are all upper case.			*/
/*									*/
/*----------------------------------------------------------------------*/
#define iabs(x) ((x)<0?-(x):(x))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/*
** Game board sizes, not easily changed.
*/

#define FONTSIZE	24	/* basic font size in bits		       */
#define BIGFONT		40	/* font size for pacman and monsters	       */
#define MAZECOLS	28	/* dimensions of the maze		       */
#define MAZEROWS	31	/* ...					       */
#define DOTS		240	/* This many dots on the maze (should compute) */

#define SCOREROW	2	/* "S C O R E : " goes here		       */
#define SCORECOL	13	/* "S C O R E : " goes here		       */
#define HIGHCOL		22	/* High score message column		       */
#define WAVEROW		2	/* "W A V E : " goes here		       */
#define WAVECOL		60	/* "W A V E : " goes here		       */
#define MAXFACES	10	/* No more than this many extra faces displayed*/
#define MAXGAMES	4	/* Maximum number of games		       */

#define PACROW		23	/* Where the pacman starts		       */
#define PACCOL		14
#define PACFACE		1	/* initial facing for pacman		       */

#define FRUITROW	17	/* position in maze coordinates of fruit       */
#define FRUITCOL	14

#define STARTROW	33	/* When monsters get here, they're ready to go */
#define STARTCOL	42	/* THESE COORDINATES MUST ALIGN!!!	       */

/*
** Game parameters
*/

#define PACMEN		3	/* initially, the number of pacmen	       */
#define	MAXMONSTER	4	/* Number of monsters			       */
#define FRUITSPERWAVE	2	/* Max fruits per wave, may be less	       */
#define WALLSDISAPPEAR  (DOTS*12)/* walls disappear if take this long */

/*
** Scheduler delays for board components.
** If these seem fast - you need more practice.
** There are 1000 clock ticks per second.
*/

#define DOTDELAY	6	/* Gobbling a dot slows down the Pacman	       */
#define EYESDELAY	20	/* For eyes moving back to the stable	       */
#define FRUITTIME	12000	/* Fruit is on this long		       */
#define MAXFRUITDELAY	(unsigned)(45000L)
				/* Fruit is scheduled before this long.	       */
#define MINFRUITDELAY	15000	/* Fruit is scheduled after this long.	       */
#define MINMONTIME	20	/* Monsters won't go faster than this	       */
#define MINPACTIME	25	/* Pacman won't go faster than this	       */
#define MINPOWERTIME	1500	/* Power pill will never last less than this   */
#define MONDELTA	3	/* Monsters get this much faster	       */
#define PACDELTA	2	/* Pacman gets this much faster		       */
#define PACTIME		47	/* Scheduler time delay for pacman	       */
#define POWERDELAY	20	/* Power pills slow monsters this much	       */
#define POWERDELTA	500	/* Power last this much less per wave	       */
#define POWERTIME	9000	/* Pacman's power, initially, lasts this long  */
#define SLOWDELTA	20	/* Monsters go Pacman.time-& in slow corridor  */
#define	STARTDELAY	500	/* Pause this long at beginning of each wave   */
#define SNDDOTDELAY	30	/* Components of gobble sound		       */

/*
** Value of board components
*/

#define SCOREMOD	10000	/* New man after Score mod & crosses zero      */
#define VALFRUIT	100	/* This much more every other wave	       */
#define VALGOLD		5	/* Ten in the arcade version - seems excessive */
#define VALMONSTER	200	/* Increases exponentially per monster caught  */
#define VALPILL		50	/* This much just for swallowing a pill	       */

/*
** To speed up the game, the score is only updated on the screen every
** 100 points.  If addscore is called with FLUSHSCORE, then the screen
** is updated immediately.
*/

#define FLUSHSCORE -1

/*
** Human factors hook - direction changes within this many units of a
** possible turn are honored - smaller values make for tricky driving.
*/

#define PENDCOUNT 20

#define NOSCORE -1

/*
** operation codes for the blt routines
*/

#define REPLACE		0
#define PAINT		1
#define	INVERT		2
#define COMPL		4
#define ERASE		8

/*
** Argument to "pacpower"
*/

#define ON		0
#define CLOSE		1
#define OFF		2

/*
** Font characters of significance
*/

#define CLOSECHAR	'q'
#define EYESCHAR	'o'
#define FACECHAR	'c'
#define FRUITCHAR	'r'
#define GHOSTCHAR	'p'
#define MONSTERCHAR	"nnvwx"
#define GOLDCHAR	'h'
#define PILLCHAR	'i'

/*
** Game Board types
**
** Wall pieces are @-F, `-f mean interior wall pieces
*/


#define TYPEMASK	017
#define DOOR		6
#define WALLMAX		6	/* Wall characters are @-F		*/
#define FIXGOLD		7	/* 'G' - this spot initialized to a dot	*/
#define FIXPILL		8	/* 'H' - initialized to a power pill	*/
#define VACANT		9	/* 'I' - initialized to empty		*/
#define SLOW		10	/* 'J' - mosters go slow here		*/
#define SAFE		11	/* 'K' - pacman invisible to monsters	*/
#define BLKDOT		12	/* 'L' - blocked to monsters & dot	*/
#define BLKNODOT	13	/* 'M' - blocked to monsters, no dot	*/
#define PILL		020
#define DISAPPEAR	040	/* set, then this is an interior wall	*/
#define GOLD		0100

/*
** Keyboard characters
*/

#define ABORT		'q'
#if V5_01 || V5_02
#define ALTDOWN		0342		/* Keypad two = down  */
#define ALTALTDOWN	0345		/* Keypad five = down, too */
#define ALTLEFT		0344		/* Keypad four = left */
#define ALTUP		0350		/* Keypad eight = up  */
#define ALTRIGHT	0346		/* Keypad six = right */
#define NOSCROLL	0360
#else
#define ALTDOWN		0301		/* Keypad two = down  */
#define ALTALTDOWN	0321		/* Keypad five = down, too */
#define ALTLEFT		0320		/* Keypad four = left */
#define ALTUP		0342		/* Keypad eight = up  */
#define ALTRIGHT	0322		/* Keypad six = right */
#endif
#define DOWN		'j'
#define GAME1		'1'
#define GAME2		'2'
#define GAME3		'3'
#define GAME4		'4'
#define GAME5		'5'
#define LEFT		'h'
#define NEWGAME		'n'
#define RIGHT		'l'
#define SILENT		's'
#define STOP		' '
#define UP		'k'
#if V5_01 || V5_02
#define LOUDER          0241        /* Up arrow  on keyboard.   */
#define SOFTER          0242        /* Down arrow on keyboard.  */
#else
#define LOUDER          0xF1        /* Up arrow  on keyboard.   */
#define SOFTER          0xF2        /* Down arrow on keyboard.  */
#endif


/*
**  Monster states (mptr->movestate)
**
**	STABLED ... bouncing around, won't leave until delay up and pacman safe.
**	LEAVING ... disjoint from STABLED because a monster might get stuck in
**		    to door is pacman swallows a pill right then.
**	HOMING .... seeking the entrance to the stable.
**	HOMING2 ... seeking the home position in the stable.
*/

#define	STABLED		1	/* In the stable, bouncing around */
#define LEAVING		2	/* Heading for the door */
#define ROAMING		3	/* roaming the maze */
#define SEEKING		4	/* seeking the pacman */
#define HOMING		5	/* seeking stable entrance */
#define HOMING2		6	/* entering stable */

/*
** values for monster state field
*/

#define DANGEROUS	1
#define EDIBLE		2
#define DEAD		3

/*
** Directions
*/

#define MOVERIGHT	1
#define MOVEUP		2
#define MOVELEFT	3
#define MOVEDOWN	4
#define MOVESTOP	0

#define BYTESIZE	8
#define RASTROWLEN	128
#define RASTCOLLEN	96

/*
** Address conversion macros
**
**	MZtoSC - maze to screen
**	SCtoMZ - screen to maze
**	SCtoROW - screen to character row
**	SCtoCOL - screen to character column
**	SCtoMEM - screen to memory
*/

/* fast times three */
#define MZtoSC(x) ((x)+((x)<<1))
#define SCtoMZ(x) ((x)/3)
#define SCtoROW(r) (((r)>>1)+3)
#define SCtoCOL(c) ((((c)<<3)/9)+2)
#if V1_25
#define SCtoMEM(r,c) (unsigned char *)(DISPLAY+((RASTCOLLEN-(c)-6)*BYTESIZE*RASTROWLEN+(r)+6))
#else
#define SCtoMEM(r,c) (unsigned char *)(DISPLAY+((((long)(c))+6)*BYTESIZE*RASTROWLEN+(RASTROWLEN-((long)(r))-6)))
#endif

/*
** TYPES...
*/

typedef struct {
	bool	alive;		/* TRUE => Pacman is alive		       */
	int	col;		/* Column position in 8x8 coordinates	       */
	int	dir;		/* velocity right(1), up(2), left(3), down(4)  */
	char	font;		/* Current font character		       */
	int	mouth;		/* Mouth state 0..6			       */
	bool	moving;
	int	pendcnt;	/* Time (in moves) remaining for pending move  */
	int	pending;	/* Pending move direction		       */
	int	power;		/* Number of power pills in effect	       */
	bool	safe;		/* TRUE => Monsters can't find him	       */
	int	row;		/* position in 8x8 coordinates		       */
	int	time;		/* Current scheduler time delay		       */
	int	mrow;		/* current position in maze coordinates	       */
	int	mcol;		/* current position in maze coordinates	       */
	int	adjusttime;	/* time adjustment factor - when eating dots   */
	int	moves;		/* elemental moves made by pacman	       */
} pacman;


typedef struct {
	int	initrow;	/* initial row				       */
	int	initcol;	/* initial column			       */
	int	initdir;	/* initial direction			       */
	int	initdelay;	/* initial time delay in the stable	       */
	int	inittime;	/* Scheduler time delay added to Monbase       */
	int	bonus;		/* bonus points in seeking pacman	       */
	/* end static part */
	int	time;		/* current scheduler delay		       */
	int	movestate;	/* monster movement state		       */
	int	state;		/* monster state			       */
	int	row;
	int	col;
	int	dir;
	int	delay;		/* delay before coming out		       */
	char	font;
	bool	canreverse;
	int	mrow;		/* maze row position			       */
	int	mcol;		/* maze column position			       */
} monster;

/*
** Fruit structure
*/

typedef struct {
	bool	alive;
	int	row;		/* Constants, defined at init time */
	int	col;
	int	value;
	char	font;
	int	thiswave;
} fruit;

/*
** Movement structure - passed around by monster move routines
*/

typedef struct {
	int row;	/* In maze coordinates */
	int col;
	int dir;
} move;

extern	unsigned char chr24[][FONTSIZE*FONTSIZE/BYTESIZE];
extern	unsigned char chr40[][BIGFONT*BIGFONT/BYTESIZE];
extern	char	*Board[];
pacman	Pacman;
extern	monster Monster[];
fruit	Fruit;
extern	long	Highscore[];
int	Seekprob;
int	Seekinc;
bool	Silent;
int	Wave;
int	Dotsrem;
bool	Monseeking;
int	Monvalue;
int	Pacmen;
int	Monbase;
int     VolumeControl;
bool	Wallsoff;
/*
** Non integer routines
*/
move *dumbmove();
move *seekmove();
move *closest();
move *bounce();
move *tostart();
move *furthest();
move *tostable();
move *todoor();
long  encrypt();

int	pacmove();
int	monmove();
int	pacpower();
long	encrypt();
