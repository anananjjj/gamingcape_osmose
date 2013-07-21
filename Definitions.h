#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/* 
	Uncomment and rebuild project for integrated debugger
   	Don't forget to uncomment OPT_OBJS line in Makefile before compiling. 
*/
//#define BUILT_IN_DEBUGGER

//#define AUTO_SCREENSHOT

/* Number of Z80 cycles for each scanline. */
#define CPU_CYCLES_PER_LINE        228

/* SDL delay every 3 frames.*/
#define DELAY_BETWEEN_FRAME        50

/* SDL Sound buffer size for samples. */
#define SAMPLE_SIZE                2048      // Sample duration is 46.4ms

/**/
#define SND_TOGGLE                 0.4026717557251908  // rest of 367.5/262.

/* Uncomment this line for verbose information on PSG/VDP access */
//#define PSG_VERBOSE
//#define VDP_ACCESS
//#define AUTO_NAT_VERBOSE
//#define PAD_VERBOSE
//#define MEM_CTRL_VERBOSE
/* Just uncomment this define to get trace of bank mapping operations */
//#define P_VERBOSE	

#endif
