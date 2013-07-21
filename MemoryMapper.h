/*****************************************************************************
*
* File: MemoryMapper.h
*
* Project: Osmose emulator.
*
* Description: This class will handle emulator read/write and bank 
* switching during emulation. 
*
* Author: Vedder Bruno
* Date: 02/10/2004, 18h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "unzip/unzip.h"
#include "Options.h"
#include "Definitions.h"
#include "DebugEventThrower.h"
#include "SaveState.h"

extern Options opt;

using namespace std;

enum Mapper
{
	SegaMapper,
	CodemasterMapper,
	KoreanMapper
};

typedef struct
{

	unsigned char paging_regs[4];
	Mapper mapperType;
    
} MemoryMapperSaveState;

class MemoryMapper : public DebugEventThrower, public ImplementsSaveState
{
public:

    MemoryMapper(const char *rom_file, const char *dud);
    void save_battery_backed_memory(string f);
    void reset();	 
    void dump(unsigned char bnk_nbr);
    void dump_page(unsigned char page);
    void dump_mem(unsigned add, unsigned short nb_line);
    void dump_smem(unsigned add, unsigned short nb_line);
    void wr8(unsigned address, unsigned char value);
    unsigned char rd8(unsigned  address);
    unsigned char getRSR();
    unsigned char getFFFD();
    unsigned char getFFFE();
    unsigned char getFFFF();
    string getROMName();
    void setMapperType (Mapper map);

	/* Implemetntation of ImplementsSaveState. */
    bool saveState( ofstream &ofs);
    bool loadState( ifstream &ifs);

private:

    bool	    	have_bbr;		// Battery backed ram flag presence  
    string 	    	rom_name;		// Rom name without extension.
    unsigned 	    rom_size;		// ROM size in bytes 		     
    unsigned        bank_nbr;		// How much 8k block in our rom	     
    unsigned        bank16Ko_nbr;	// How much 16k bank in our rom	         
    unsigned char   *null_rom;		// Ptr on Garbage (use for ROM writes).
    unsigned char   *sram;			// Ptr on Optionnal SRAM
    unsigned char   *ram;			// Central RAM 
    unsigned char   *cartridge;		// Ptr on ROM cartridge.
    unsigned char   paging_regs[4];     // Paging registers.
    unsigned char   *read_map[8];	// 8ko bank ptr for CPU reads.
    unsigned char   *write_map[8];	// 8ko bank ptr for CPU reads.    
    bool save_bbr;			// Flag for Battery Backed Memory.    
    Mapper mapperType;

    unsigned int LoadZippedRom(const char *rom_file);
    unsigned int LoadSMSRom(const char *rom_file);
    void DumpMappingPtr();
	void wr8_sega_mapper(unsigned int add, unsigned char data);
	void wr8_codemaster_mapper(unsigned int add, unsigned char data);
	void wr8_korean_mapper(unsigned int add, unsigned char data);
    void write_standard_paging_reg(int reg, unsigned char value);    
    void write_codemaster_paging_reg(unsigned char value);    
    void DisplayROMSize();
    unsigned int getCRC32(unsigned char *buffer, unsigned int len);
    void (MemoryMapper::*wr8_method)(unsigned int add, unsigned char data);
};
#endif
