#define VSIZE		0x300
#define HSIZE		0x400

#define ROMSTART	0x000000	/* Beginning of ROM		*/
#define ROMEND		0x018000	/* End       of ROM		*/
#if V5_01 || V5_02
#define RAMSTART	0x200000	/* Beginning of RAM		*/
#define DISPLAY		0x260000	/* Beginning of visible RAM	*/
#define EDISPLAY	0x278000	/* End of visible RAM		*/
#endif
#if V3_10
#define RAMSTART	0x380000	/* Beginning of RAM		*/
#define DISPLAY		0x3e0000	/* Beginning of visible RAM	*/
#define EDISPLAY	0x3f8000	/* End of visible RAM		*/
#endif
#define PSG_RMAX        (1<<4)      /* Number of PSG registers.              */
#define PSG_TMAX        (1<<12)     /* Maximum tone  period register value.  */
#define PSG_NMAX        (1<<5)      /* Maximum noise period register value.  */
#define PSG_VMAX        (1<<4)      /* Maximum volume       register value.  */
#define PSG_EMAX        ((unsigned)(1<<16))
				    /* Maximum envelope period  value.       */
#define PSG_FREQ        (1294400L/16)
		                    /* PSG clocking frequency.               */
#define PSG_NPERIOD     0x06        /* PSG noise period register.            */
#define PSG_ENABLE      0x07        /* PSG enable register.                  */
#define  PSG_0TONE      0x01        /* PSG enable channel 0 tone.            */
#define  PSG_1TONE      0x02        /* PSG enable channel 1 tone.            */
#define  PSG_2TONE      0x04        /* PSG enable channel 2 tone.            */
#define  PSG_0NOISE     0x08        /* PSG enable channel 0 noise.           */
#define  PSG_1NOISE     0x10        /* PSG enable channel 1 noise.           */
#define  PSG_2NOISE     0x20        /* PSG enable channel 2 noise.           */
#define  PSG_AOUTPUT    0x40        /* PSG enable bus A for output.          */
#define  PSG_BOUTPUT    0x80        /* PSG enable bus B for output.          */
#define PSG_0VOLUME     0x08        /* PSG channel 0 volume setting.         */
#define PSG_1VOLUME     0x09        /* PSG channel 1 volume setting.         */
#define PSG_2VOLUME     0x0A        /* PSG channel 2 volume setting.         */
#define PSG_EPERIOD     0x0B        /* PSG envelope period register.         */
#define PSG_SHAPE       0x0D        /* PSG envelope shape control register.  */

#define PSG_CONTINUE    0x08        /* PSG continue cycling bit.             */
#define PSG_ATTACK      0x04        /* PSG attack rather than decay.         */
#define PSG_ALTERNATE   0x02        /* PSG alternate attack/decay.           */
#define PSG_HOLD        0x01        /* PSG hold last count after one cycle.  */
#define PSG_EENABLE     0x10        /* OR this bit into volume for envelope. */
