/*****************************************************************************
*
* File: RomSpecificOption.cpp
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
#include "RomSpecificOption.h"
#include "MemoryMapper.h"

/* Osmose embedded Database. */
const RSO_DB_Entry SpecificRomOptionDatabase::db[] = 
{
	/* SMS ROMS. */
	{ 0x8dbdf0c7, VIDEO_PAL}, 			/* Addams Family, The (UE) [!].sms  */
	{ 0x06244acc, SPORT_PADDLE_INPUT},	/* Alex Kidd BMX Trial (UE) [!].sms */
	{ 0xd2b73e2c, VIDEO_PAL},			/* Back to the Future 3 (UE) [!]    */
	{ 0x3f1da29d, VIDEO_PAL},			/* California Games 2 (UE) [!].sms  */
	{ 0xa0a2ec50, VIDEO_PAL},			/* California Games 2 (UE) [b1].sms */
	{ 0x76486188, KOREAN_MAPPER}, 		/* Dodgeball King (Korean).sms*/
	{ 0x5905bd2f, SPORT_PADDLE_INPUT},  /* Galactic Protector (UE) [!].sms  */
	{ 0xd6438052, SPORT_PADDLE_INPUT | JAPANESE_MACHINE},  /* Megumi Rescue (J) [!].sms		*/
	{ 0xffb849ea, VIDEO_PAL},			/* Predator 2 (UE) [!].sms 			*/
	{ 0x682fcabe, KOREAN_MAPPER},		/* Sangokushi 3 (Korean).sms 		*/
	{ 0x57f9e510, INTERRUPT_HACK},		/* Sega Chess (UE) [!].sms			*/
	{ 0xcea6e82b, SPORT_PADDLE_INPUT | JAPANESE_MACHINE}, /* Woody Pop (J) [!].sms */
	{ 0x6262f5a0, INTERRUPT_HACK}, 		/* Zool (UE) [!].sms  */

	/* Game Gear ROMS. */
	{ 0x94f0313c, INTERRUPT_HACK},		/* Chicago Syndicate (JUE) [!].gg	*/
	{ 0xead0f233, CODE_MASTER_MAPPER},	/* Drop Zone (U) [!].gg 			*/
	{ 0xbac3a313, INTERRUPT_HACK},      /* Monster Truck Wars (JUE) [!]		*/
	{ 0x3e8a9411, CODE_MASTER_MAPPER},  /* Pete Sampras Tennis (E) [!].gg 	*/
	{ 0x26580e8f, CODE_MASTER_MAPPER},  /* S.S. Lucifer - Man Overboard! (UE) [!].gg*/
	{ 0x1ca10812, INTERRUPT_HACK}, 		/* Zool (J).gg */
	{ 0x00000000, 0}, 			  		/* Use as end of list */
};

/* This method return specific options if given CRC match database CRC. */
/* Zero is returned if no CRC is found, meaning no specific options.    */
unsigned int SpecificRomOptionDatabase::getOptions(unsigned int rom_crc32,Options *op)
{
    bool rom_identified = false;
	unsigned int index  = 0;
    unsigned int option = 0;
	
	/* Search in db until CRC and specific_options = 0 (end of list) or CRC is found. */
	while ((rom_identified == false) && ((db[index].crc32 != 0)&&(db[index].specific_options != 0)))
	{
		if (db[index].crc32 == rom_crc32)
		{
			option = db[index].specific_options ;
			rom_identified = true;
		}
		index++;
	}

	/* Now set the specific options. */
	if (option & INTERRUPT_HACK)
	{
	    op->irq_hack   = true;
		cout << " * IRQ Hack ON." << endl;
	}

	if (option & CODE_MASTER_MAPPER) 
	{
		op->mapperType = CodemasterMapper;
		cout << " * Using Codemaster mapper." << endl;
	}

	if (option & KOREAN_MAPPER) 
	{
		op->mapperType = KoreanMapper;
		cout << " * Using Korean mapper." << endl;
	}

	if (option & VIDEO_PAL)
	{
		op->ntsc = false;
		cout << " * Using PAL video mode." << endl;
	}

	if (option & SPORT_PADDLE_INPUT) 
	{
		op->inputType = PADDLE;
		cout << " * Using SPORT PADDLE input device." << endl;
	}

	if (option & JAPANESE_MACHINE) 
	{
		op->inputType = PADDLE;
		op->WorldVersion = JAPAN;
		cout << " * Using JAPANESE SMS." << endl;
	}
	return option;
}
