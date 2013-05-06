/*****************************************************************************
* file: bg.h                                                                 *
*                                                                            *
* Copyright (c) 1981 by Bolt Beranek and Newman Inc.                         *
* All rights reserved                                                        *
*                                                                            *
*   Contents:     BBN BitGraph terminal header file                          *
*   Authors:      Robert E. Wells and Rich Fortier.                          *
*   Created:      February, 1981                                             *
*                                                                            *
*   Remarks:                                                                 *
*                                                                            *
*****************************************************************************/

#ifndef PARMNONE

#define HEIGHT      HSIZE           /*   height is HSIZE and        */
#define WIDTH       VSIZE           /*     width is VSIZE.          */


#define INTSIZE     2               /* Size of an int in bytes.     */
#define PARMNONE    0x8001          /* No parameter specified.      */
#define PARMDFLT    0x8000          /* Parameter defaulted.         */
#define argoff(n, off) [n*2 + off]  /* Macro for accessing args.    */
#define clrint      clrw            /* Clear   a short int.         */
#define cmpint      cmpw            /* Compare a short int.         */
#define movint      movw            /* Move    a short int.         */
#define tstint      tstw            /* Test    a short int.         */

#define NUL         0
#define SOH         1
#define STX         2
#define ETX         3
#define EOT         4
#define ENQ         5
#define ACK         6
#define BELL        7
#define BS          8
#define TAB         9
#define LF          10
#define VT          11
#define FF          12
#define CR          13
#define SO          14
#define SI          15
#define DLE         16
#define DC1         17
#define   XON       DC1
#define DC2         18
#define DC3         19
#define   XOF       DC3
#define DC4         20
#define NAK         21
#define SYN         22
#define ETB         23
#define CAN         24
#define EM          25
#define SUB         26
#define ESC         27
#define FS          28
#define GS          29
#define RS          30
#define US          31
#define DEL         127


#define RASTCLR     0           /* RASTOP code: 0                            */
#define  RASTNSET   0           /* RASTOP code: 0                            */
#define RASTSAD     1           /* RASTOP code: s & d                        */
#define  RASTNERASE 1           /* RASTOP code: s & d                        */
#define RASTSAND    2           /* RASTOP code: s & ~d                       */
#define RASTS       3           /* RASTOP code: s                            */
#define  RASTRPL    3           /* RASTOP code: s                            */
#define RASTNSAD    4           /* RASTOP code: ~s & d                       */
#define  RASTERASE  4           /* RASTOP code: ~s & d                       */
#define RASTD       5           /* RASTOP code: d                            */
#define  RASTNOP    5           /* RASTOP code: d                            */
#define RASTSXD     6           /* RASTOP code: s ^ d                        */
#define  RASTXOR    6           /* RASTOP code: s ^ d                        */
#define RASTSOD     7           /* RASTOP code: s | d                        */
#define  RASTDRAW   7           /* RASTOP code: s | d                        */
#define RASTNSAND   8           /* RASTOP code: ~s & ~d --> ~(s | d)         */
#define RASTSXND    9           /* RASTOP code: s ^ ~d                       */
#define  RASTNXOR   9           /* RASTOP code: s ^ ~d                       */
#define RASTND      10          /* RASTOP code: ~d                           */
#define  RASTCOM    10          /* RASTOP code: ~d                           */
#define  RASTNCOM   10          /* RASTOP code: ~d                           */
#define RASTSOND    11          /* RASTOP code: s | ~d                       */
#define RASTNS      12          /* RASTOP code: ~s                           */
#define  NRPL       12          /* RASTOP code: ~s                           */
#define RASTNSOD    13          /* RASTOP code: ~s | d                       */
#define  RASTNDRAW  13          /* RASTOP code: ~s | d                       */
#define RASTNSOND   14          /* RASTOP code: ~s | ~d --> ~(s & d)         */
#define RASTSET     15          /* RASTOP code: 1                            */
#define  RASTNCLR   15          /* RASTOP code: 1                            */

#define BGMODE      0           /* Emulate BitGraph Terminal; native mode.   */
#define VT52MODE    1           /* Emulate DEC VT52  terminal.               */
#define VT100MODE   2           /* Emulate DEC VT100 terminal.               */ 
#define TEKMODE     3           /* Emulate Tektronix 4010 terminal.          */

#define KBDNSCRL    0xB0        /* NO SCROLL key on keyboard                 */
#define KBDBREAK    0xE0        /* BREAK key on keyboard.                    */
#define KBDSHFTBRK  0xEA        /* SHIFT-BREAK on keyboard.                  */
#define KBDSETUP    0xFE        /* SETUP key on keyboard.                    */
#define KBDUP       0xF1        /* Up arrow  on keyboard.                    */
#define KBDDOWN     0xF2        /* Down arrow on keyboard.                   */
#define KBDLEFT     0xF3        /* Left arrow on keyboard.                   */
#define KBDRIGHT    0xF4        /* Right arrow on keyboard.                  */
#define KBDBELL     4           /* Long beep for keyboard.                   */
#define KBDLOCAL    3           /* LOCAL light for keyboard.                 */
#define KBDLINE     1           /* LINE  light for keyboard.                 */
#define KBDCLICK    0           /* Program keyboard for    KEYCLICK.         */
#define KBDNOCLICK  8           /* Program keyboard for no KEYCLICK.         */

#define PPURESET    0           /* Reset the peripheral processor.           */
#define PPUCHECK    1           /* Check peripheral by looping data.         */ 
#define PPUMENABLE  2           /* Enable  mouse reporting.                  */
#define PPUMDISABLE 3           /* Disable mouse reporting.                  */
#define PPUJENABLE  4           /* Enable joystick reporting.                */
#define PPUJDISABLE 5           /* Disable joystick reporting.               */
#define PPUPRINT    6           /* Print characters on Centronic port.       */
#define PPUMCOOR    10          /* Mouse coordinate report.                  */
#define PPUJCOOR    11          /* Joystick coordinate report.               */
#define PPUPDONE    12          /* Printer completion report                 */

#define STACKSIZE   (768)       /* Number of bytes reserved for stack space  */
#define Q_SIZE      18          /* size of a queue header                    */
#define MAXLINE     63          /* maximum line number in character coords   */
#define MAXCOL      84          /* maximum column number in character coords */
#define COLSIZE     0x80        /* size of each column (in addresses)        */

#define HOST_RLEN   1024        /* size of host read queue                   */
#define HOST_WLEN   128         /* size of host write queue                  */
#define KBD_RLEN    30          /* size of user typein queue                 */
#define KBD_WLEN    0x11        /* size of keyboard output queue             */

#define JMP         0x4ef9      /* absolute jump instruction                 */
#define CURSORBIT   0x200       /* real time clock bit for enabling cursor   */
#define DEADMAN     10000/1     /* deadman restart if no chars read for 10s  */
#define M_CLEANBIT  1           /* Bit  to test for clean mode bits.         */
#define M_CLEANMASK 2           /* Mask to test for clean mode bits.         */

#ifndef IS

#define IS      ==           /* cosmetic defintions */
#define ISNT    !=
#define NOT     !
#define YES     1
#define NO      0
#define OK      0
#define ERROR   -1
#define AND     &&
#define OR      ||

#define forever         for(;;)
#define FOREVER         for(;;)
#define plural(i)       ( ((i) IS 1) ? "":"s")
#define intern
#define auto
#endif

#ifndef NULL
#define NULL 0
#endif

#endif 
