/*****************************************************************************
*
* File: RomSpecificOption.h
*
* Project: Osmose emulator.
*
* Description: This class will store ROM CRC32 and specific options for
* rom that need them to run.
*
* Author: Vedder Bruno
* Date: 22.11.2006 13:08:54 
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef ROM_SPECIFIC_OPTION_H
#define ROM_SPECIFIC_OPTION_H
#include "Options.h"
#include <iostream>

enum 
{
	CODE_MASTER_MAPPER 	= 0x1,
	KOREAN_MAPPER		= 0x2,
	VIDEO_PAL			= 0x4,
	INTERRUPT_HACK	   	= 0x8,
	SPORT_PADDLE_INPUT  = 0x10,
	JAPANESE_MACHINE    = 0x20
};

/* Entry into our database. */
typedef struct 
{
	unsigned int crc32;
	unsigned int specific_options;
} RSO_DB_Entry;

class SpecificRomOptionDatabase
{
public:

	static unsigned int getOptions(unsigned int rom_crc32, Options *o);

private:

	/* Private Constructor. This class is never instanciated. */
	SpecificRomOptionDatabase();
	static const RSO_DB_Entry db[];	
};


#endif
