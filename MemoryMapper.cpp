/*****************************************************************************
*
* File: MemoryMapper.cpp
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
#include "MemoryMapper.h"
#include "Options.h"
#include "RomSpecificOption.h"
#include <cstdlib>
#include <cstring>
#include <stdio.h>

extern Options opt;

/*--------------------------------------------------------------------*/
/* Class constructor. It immediatly load a given rom, and updates     */
/* Memory mapper variables.					      */
/*--------------------------------------------------------------------*/
MemoryMapper::MemoryMapper(const char *rom_file, const char *uhd)
{
    save_bbr       = false;
    string n = string(rom_file);
    string ext;
    int last_separator_index = -1;
    unsigned int rom_crc;

    if (n.length() >=5)
    {
	    ext = n.substr(n.length()-3,3);
    }
    else
    {
		cerr << "Invalid ROM name."<< endl;
		cerr << "Valid name is at least one letter, and extension. Ex: a.sms or b.zip "<< endl;
		exit (-1);
    }

    // Load SMS unzipped file.
    if (ext == "sms" || ext == "SMS")
    {
        rom_crc = LoadSMSRom(rom_file);
        opt.MachineType = SMS;
        cout << "Switching emulator to SMS mode." << endl;
    }
    else
    if (ext == ".gg" || ext == ".GG") // Load GAMEGEAR unzipped file.
    {
        rom_crc = LoadSMSRom(rom_file);
        opt.MachineType = GAMEGEAR;
        cout << "Switching emulator to GAMEGEAR mode." << endl;
    }
    else
    if (ext == "zip" || ext == "ZIP")
	{
	    rom_crc = LoadZippedRom(rom_file);
	}
	else
	{
	    cerr << "Unknown file extension: " << ext << endl;
	    cerr << "Known extensions supported by Osmose are: .sms .gg or .zip " << endl;
	    exit (-1);
	}

	/* Print the ROM CRC. */
    cout << "CRC32 = 0x"<< hex << setfill('0') << setw(8) << rom_crc << endl;

	/* Now check in RomSpecificDataBase if we need specific option to run the rom. */
    unsigned int options = SpecificRomOptionDatabase::getOptions( rom_crc, &opt );
    if (!options) cout << "This game does not need specific options." << endl;

    // Now extract the rom name.
    for (unsigned int i=0; i < n.length(); i++)
    {
        if (n[i] == '\\' || n[i] == '/')
	    {
	        last_separator_index =i;
	    }
    }
    
    int ext_lenght = 4;
    if (opt.MachineType == GAMEGEAR) ext_lenght = 3;
    rom_name = n.substr(last_separator_index+1, n.length() - (last_separator_index+1) - ext_lenght);

    // Allocate Central RAM (0xC000-0xDFFF mirrored)
    ram = new unsigned char[0x2000];
    if (ram==NULL)
    {
        cerr << "Unable to allocate 8Ko memory for onboard RAM." << endl;
        cerr << "Exiting." << endl;
	exit(-1);
    }
    else
    {
        // Clear this RAM.
	for (int i=0; i<0x2000; i++)
	{
	    ram[i] = 0;
	}
    }

    // Allocate 32Ko RAM banks, (2*16 opt banks) for games that use it.
    sram = new unsigned char[0x8000];
    if (sram==NULL)
    {
        cerr << "Unable to allocate 32Ko memory for optionnal RAM." << endl;
        cerr << "Exiting." << endl;
	exit(-1);
    }
    else
    {
        bool restored_old_bbr = false;
	    char full_name[256];

        // Are there any battery backed memory save ?
#ifdef __USE_UNIX98
        sprintf(full_name,"./osmose_files/bbr/%s%s", rom_name.c_str(),".bbr");
#endif
#ifdef __WIN32
        sprintf(full_name,".\\bbr\\%s.bbr", rom_name.c_str());
#endif

	ifstream file(full_name, ios::in | ios::binary);
	if (file.is_open() == true )
	{
	    // Now load 2*16Ko banks.
	    file.read((char *)sram,0x8000);
	    file.close();
	    restored_old_bbr = true;
	    cout << "Restored Battery Backed Ram (.bbr) file:" << full_name << "." << endl;
	}

        if (restored_old_bbr != true)
        {
            cout << "Battery Backed Ram \'"<< rom_name.c_str() <<"\' does not exist." << endl;
            // No battery backed memory, clear this Optional RAM.
		    for (int i=0; i<0x8000; i++)
		    {
				sram[i] = 0x0;
		    }
        }
    }
    //dump_smem(0, 16*20);
    // Allocate 8Ko of ram for writes in rom.
    null_rom = new unsigned char[0x2000];
    if (sram==NULL)
    {
        cerr << "Unable to allocate 8Ko memory for optionnal RAM." << endl;
        cerr << "Exiting." << endl;
	exit(-1);
    }

    /* Init wr8_method pointer on default sega mapper. */
	wr8_method = &MemoryMapper::wr8_sega_mapper;
    mapperType = SegaMapper;

	/* Rom is loaded, reset the MemoryMapper. */
	reset();
}

/*------------------------------------------------------------*/
/* This method reset paging to it's supposed initial value:   */
/* page 0 on first rom bank, page1 on bank1 and so on.	      */
/*------------------------------------------------------------*/
void MemoryMapper::reset()
{

    // Init Read map and Write map.

    read_map[0] = &cartridge[0x0000]; // 0x0000-0x1FFF
    read_map[1] = &cartridge[0x2000]; // 0x2000-0x3FFF

    read_map[2] = &cartridge[0x4000]; // 0x4000-0x5FFF
    read_map[3] = &cartridge[0x6000]; // 0x6000-0x7FFF

    read_map[4] = &cartridge[0x8000]; // 0x8000-0x9FFF
    read_map[5] = &cartridge[0xA000]; // 0xA000-0xBFFF

    read_map[6] = &ram[0];      // 0xC000-0xDFFF
    read_map[7] = &ram[0];      // 0xE000-0xFFFF mirror of 0xC000-0xDFFF


    write_map[0] = &null_rom[0]; // 0x0000-0x1FFF
    write_map[1] = &null_rom[0]; // 0x2000-0x3FFF

    write_map[2] = &null_rom[0]; // 0x4000-0x5FFF
    write_map[3] = &null_rom[0]; // 0x6000-0x7FFF

    write_map[4] = &null_rom[0]; // 0x8000-0x9FFF
    write_map[5] = &null_rom[0]; // 0xA000-0xBFFF

    write_map[6] = &ram[0];	 // 0xC000-0xDFFF
    write_map[7] = &ram[0];	 // 0xE000-0xFFFF mirror of 0xC000-0xDFFF

    // Init pagin registers From FFFC-FFFF.
    paging_regs[0]  = 0;
    paging_regs[1]  = 0;
    paging_regs[2]  = 1;
    paging_regs[3]  = 2;
}

/*------------------------------------------------------------*/
/* This is a debugging purpose function. 		      */
/* Note that this method will dump banks without any banking  */
/* purpose. It's not affected by paging. To dump paged bank   */
/* use dump_page method.				      */
/*------------------------------------------------------------*/
void MemoryMapper::dump(unsigned char bank_n)
{

    int adress = 0;
    if (bank_n >= bank_nbr)
    {
        cout << "Warning, cannot dump bank " << bank_n << " This bank does not exists." << endl;
        return;
    }

    cout << "***Dumping rom bank[" << (int)bank_n << "] (File offset is 0x" << hex << setw(8) << setfill('0') << (bank_n * 16384) << ")." << endl;
    for (int i=0; i<1024; i++)
    {
	cout << hex << setw(4) << setfill('0') << (i*16)  << ": ";
	for (int o=0;o<16;o++)
	{
	    cout << hex << setw(2) << setfill('0') <<(int) read_map[bank_n][adress++] << " ";
	}
        cout << endl;
    }
}

/*------------------------------------------------------------*/
/* This is a debugging purpose function. 		      */
/* Note that this method will dump banks with banking purpose.*/
/* It dumps data mapped into pages.			      */
/*------------------------------------------------------------*/
void MemoryMapper::dump_page(unsigned char bank_n)
{
    int adress = 0;
    if (bank_n > 2)
    {
        cout << "Warning, cannot dump page " << bank_n << " This page does not exists." << endl;
        return;
    }

    cout << "Dumping rom page " << (int)bank_n << endl;
    for (int i=0; i<1024; i++)
    {
	cout << hex << setw(4) << setfill('0') << (i*16)  << ": ";
	for (int o=0;o<8;o++)
	{
	    cout << hex << setw(2) << setfill('0') <<(int) read_map[bank_n][adress++] << " ";
	}
        cout << endl;
    }
}

/*------------------------------------------------------------*/
/* This is a debugging purpose function. 		      */
/* Note that this method will memory using env rd functions.  */
/*------------------------------------------------------------*/
void MemoryMapper::dump_mem(unsigned add, unsigned short nb_line)
{
    for (int i=0; i<nb_line; i++)
    {
	// Write address
	cout << hex << setw(4) << setfill('0') << ((add + i*16)& 0xFFFF)  << ": ";
	for (int o=0;o<16;o++)
	{
	    cout << hex << setw(2) << setfill('0') <<(int) rd8( (add+i*16+o) & 0xFFFF) << " ";
	}
        cout << endl;
    }
}

/*------------------------------------------------------------*/
/* This is a debugging purpose function. 		      */
/* Note that this method will memory using env rd functions.  */
/*------------------------------------------------------------*/
void MemoryMapper::dump_smem(unsigned add, unsigned short nb_line)
{
    add &=0x7fff;
    for (int i=0; i<nb_line; i++)
    {
	// Write address
	cout << hex << setw(4) << setfill('0') << ((add + i*16)& 0xFFFF)  << ": ";
	for (int o=0;o<16;o++)
	{
	    cout << hex << setw(2) << setfill('0') <<(int) sram[(add+i*16+o) & 0x7FFF] << " ";
	}
        cout << endl;
    }
}

/*------------------------------------------------------------*/
/* This method wraps write to the selected wr8 method         */
/* The pointer must be initialised with setMemoryMapper		  */
/* to use non default mapper.								  */
/*------------------------------------------------------------*/
void MemoryMapper::wr8(unsigned address, unsigned char value)
{
    (this->*wr8_method)( address, value);
}

/*------------------------------------------------------------*/
/* This method handle every write operations done by the CPU  */
/* accordingly to korean memory mapper.						  */
/* It handles bank switching, RAM/ROM writes.  	              */
/*							                                  */
/* Note that address is already anded with 0xFFFF	          */
/*------------------------------------------------------------*/
void MemoryMapper::wr8_sega_mapper(unsigned address, unsigned char value)
{
    int bnk = (address >> 13);
	write_map[bnk][address & 0x1FFF] = value;
	if (address >= 0xFFFC)
	{
        write_standard_paging_reg(address & 3, value);
	}
}


/*------------------------------------------------------------*/
/* This method handle every write operations done by the CPU  */
/* accordingly to codeemaster memory mapper.				  */
/* It handles bank switching, RAM/ROM writes.  	              */
/* write at 0x8000 changes rom banking.                        */
/*							                                  */
/* Note that address is already anded with 0xFFFF	          */
/*------------------------------------------------------------*/
void MemoryMapper::wr8_codemaster_mapper(unsigned address, unsigned char value)
{
    int bnk = (address >> 13);
	if (address != 0x8000)
	{
  	    write_map[bnk][address & 0x1FFF] = value;
	}
	else
	{
	    write_codemaster_paging_reg(value);
	}
}

/*------------------------------------------------------------*/
/* This method handle every write operations done by the CPU  */
/* accordingly to codemaster memory mapper.				  */
/* It handles bank switching, RAM/ROM writes.  	              */
/* write at 0xA000 changes rom banking page 2.                */
/*							                                  */
/* Note that address is already anded with 0xFFFF	          */
/*------------------------------------------------------------*/
void MemoryMapper::wr8_korean_mapper(unsigned address, unsigned char value)
{
    int bnk = (address >> 13);
	if (address != 0xA000)
	{
  	    write_map[bnk][address & 0x1FFF] = value;
	}
	else
	{
		int page = value % bank16Ko_nbr;

		/* Keep copy of 0xA000 paging reg, swap banks.*/
		paging_regs[0] = value;
	    read_map[4] = &cartridge[(page<<14)];
	    read_map[5] = &cartridge[(page<<14)+0x2000];
	    write_map[4] = null_rom;
	    write_map[5] = null_rom;
	}
}


/*------------------------------------------------------------*/
/* This method handle every read operations done by the CPU.  */
/* It handles bank switching, RAM/ROM reads.  	              */
/* Note that address is already anded with 0xFFFF	      */
/*------------------------------------------------------------*/
unsigned char MemoryMapper::rd8(unsigned  address)
{
    unsigned char r=0;
    int bnk = address>> 13; 		// bnk is 0-7.

    if (address < 0x400)
    {
        r = cartridge[address & 0x1FFF];
    }
    else
    {
        r = read_map[bnk][address & 0x1FFF];
    }
    return r;
}


/*------------------------------------------------------------*/
/* This method is in charge of memory mapping.  	      */
/* reg is the mapping register from FFFC-FFFF		      */
/* Value is the value written to this register.		      */
/*------------------------------------------------------------*/
void MemoryMapper::write_standard_paging_reg(int reg, unsigned char value)
{
    int page;
    page = (value % bank16Ko_nbr);
    paging_regs[reg] = value;	// Save paging registers.

    switch(reg)
    {
	// 0xFFFC is written.
        case 0:
            if(value & 8) // If true, An additionnal 16 Ko ram is mapped at 0x8000-BFFF
            {
				// If value & bit 4 is true, we map second 16 Bank.
				// else, it's the first.
				save_bbr = true;  // Save flag for BBR.
				read_map[4]  =  &sram[(value & 4) ? 0x4000 : 0x0000];
                read_map[5]  =  &sram[(value & 4) ? 0x6000 : 0x2000];
                write_map[4] =  &sram[(value & 4) ? 0x4000 : 0x0000];
                write_map[5] =  &sram[(value & 4) ? 0x6000 : 0x2000];
            }
            else
            {
                // We are mapping rom from 0xFFFF register.
                read_map[4]  = &cartridge[((paging_regs[3] % bank16Ko_nbr) << 14) + 0x0000];
                read_map[5]  = &cartridge[((paging_regs[3] % bank16Ko_nbr) << 14) + 0x2000];
                write_map[4] = null_rom;
                write_map[5] = null_rom;
            }
		break;

		// 0xFFFD is written, changing Page 0
        case 1:
#ifdef P_VERBOSE
            cout << "Mapping page0 on rom bank " << (unsigned int)(page) << endl;
#endif
            read_map[0] = &cartridge[(page<<14)];
	    	read_map[1] = &cartridge[(page<<14)+0x2000];
		break;

		// 0xFFFE is written.
        case 2:
#ifdef P_VERBOSE
            cout << "Mapping page1 on rom bank " << (unsigned int)(page) << endl;
#endif
            read_map[2] = &cartridge[(page<<14)];
	    	read_map[3] = &cartridge[(page<<14)+0x2000];
		break;

		// 0xFFFF is written.
        case 3:
            if(!(paging_regs[0] & 0x08))
	    	{
#ifdef P_VERBOSE
            	cout << "Mapping page2 on rom bank " << (unsigned int)(page) << endl;
#endif
                read_map[4] = &cartridge[(page<<14)];
	        	read_map[5] = &cartridge[(page<<14)+0x2000];
	    	}
		break;
    }
}

/*------------------------------------------------------------*/
/* This method is in charge of codemaster memory mapping.     */
/* To use this mapping, emu must be called with -cm option.   */
/* Value is the value written to this register.		      */
/*------------------------------------------------------------*/
void MemoryMapper::write_codemaster_paging_reg(unsigned char value)
{
    int page = value % bank16Ko_nbr;

	/* Keep a copy of 0x8000, swap banks. */
	paging_regs[0] = value;
    read_map[4] = &cartridge[(page<<14)];
    read_map[5] = &cartridge[(page<<14)+0x2000];
    write_map[4] = null_rom;
    write_map[5] = null_rom;
}

/*------------------------------------------------------------*/
/* This method returns RAM Select Register mapped at 0xFFFC   */
/*------------------------------------------------------------*/
unsigned char MemoryMapper::getRSR()
{
    return paging_regs[0];
}

/*------------------------------------------------------------*/
/* This method returns bank selector apped at 0xFFFD   	      */
/*------------------------------------------------------------*/
unsigned char MemoryMapper::getFFFD()
{
    return paging_regs[1];
}

/*------------------------------------------------------------*/
/* This method returns bank selector apped at 0xFFFE   	      */
/*------------------------------------------------------------*/
unsigned char MemoryMapper::getFFFE()
{
    return paging_regs[2];
}

/*------------------------------------------------------------*/
/* This method returns bank selector apped at 0xFFFF   	      */
/*------------------------------------------------------------*/
unsigned char MemoryMapper::getFFFF()
{
    return paging_regs[3];
}

/*------------------------------------------------------------*/
/* This is a debug method, not used during normal emulation.  */
/*------------------------------------------------------------*/
void MemoryMapper::DumpMappingPtr()
{
//    for (int i=0; i <8;i++)
//    {
//        cout << "readMap[" << i << "]=" << (unsigned)read_map[i] << endl;
//    }
}

/*------------------------------------------------------------*/
/* This method loads Zipped Rom into cartridge memory         */
/*------------------------------------------------------------*/
unsigned int MemoryMapper::LoadZippedRom(const char *rom_file)
{
    unzFile myZip = NULL;
    unz_global_info zip_global_info;
    unz_file_info zip_file_info;
    unsigned char *dummy=NULL;		// Used to skip first 512 bytes.
    char filename[256];			// Name of compressed file.
    unsigned int file_nbr = 0;
    int ret;
    bool smsArchiveFound = false;

    // Allocate dummy buffer to skip potential 512 bytes rom header.
    dummy = (unsigned char*) malloc(512);
    if (dummy == NULL)
    {
        cerr << "Unable to allocate dummy 512 bytes buffer." << endl;
	exit (-1);
    }

    // Open ROM with file pointer at the end of ROM.
    myZip = unzOpen(rom_file);
    if (myZip == NULL)
    {
        cerr << "Unable to open " << rom_file << " zip archive." << endl;
	exit (-1);
    }

    // Get global information on zip.
    ret = unzGetGlobalInfo(myZip, &zip_global_info);
    if (ret == UNZ_OK)
    {
	if (zip_global_info.number_entry != 1)
	{
	    printf("Warning Found more than one file in archive.\n");
	}
    }
    else
    {
	printf("unzGetGlobalInfo() call failed.\n");
	exit(-1);
    }

    // Get First file in zip archive.
    ret = unzGoToFirstFile(myZip);
    if (ret != UNZ_OK)
    {
	printf("unzGoToFirstFile() call failed.\n");
	exit(-1);
    }

    do
    {
	// Read global information on the file.
	ret = unzGetCurrentFileInfo(myZip, &zip_file_info,
    				    filename, 256, NULL,0, NULL, 0);
	// Get Sms file original size.
	if (ret == UNZ_OK)
	{
            rom_size = zip_file_info.uncompressed_size;

	    // Check Extension: should be .sms .gg .GG or .SMS to be accepted.
	    if ((strncasecmp(&filename[strlen(filename)-4],".sms", 4)== 0))
	    {
            cout << "File in zip archive is " << filename << endl;
		    smsArchiveFound = true;
		    opt.MachineType = SMS;
            cout << "Switching emulator to SMS mode." << endl;
	    }
	    if ((strncasecmp(&filename[strlen(filename)-3],".gg", 3)== 0))
	    {
            cout << "File in zip archive is " << filename << endl;
		    smsArchiveFound = true;
            opt.MachineType = GAMEGEAR;
            cout << "Switching emulator to GAMEGEAR mode." << endl;
	    }
	    file_nbr++;
	}
	else
	{
	    printf("unzGetCurrentFileInfo() call failed.\n");
	    exit(-1);
	}

	if (smsArchiveFound != true && file_nbr <  zip_global_info.number_entry)
	{
	    ret = unzGoToNextFile (myZip);
	    if (ret != UNZ_OK)
	    {
		printf("unzGoToNextFile() call failed.\n");
		exit (-1);
	    }
        }

    } while (smsArchiveFound == false  &&  file_nbr <  zip_global_info.number_entry);

    if (smsArchiveFound != true)
    {
	printf("The .zip archive does not contain '.sms' or .'gg' file.\nAborting.\n");
	exit (-1);
    }

    // Now open rom file, prepare for reading.
    ret = unzOpenCurrentFile(myZip);
    if (ret != UNZ_OK)
    {
	printf("Unable to open file from zip archive.\n");
	exit(-1);
    }

    // Some rom seems to have a 512byte header. Skip it.
    // unzip package doesn't handle seek. So read buffer to skip it.
    if ((rom_size %1024) == 512)
    {
        cout << "512 bytes ROM header Skipped."<< endl;
	ret = unzReadCurrentFile(myZip, dummy, 512);
	if (ret < 0)
	{
            cerr << "Unable read 512 bytes from zip archive." << endl;
	    exit (-1);
	}
	rom_size -= 512;
    }

    DisplayROMSize();

    // Save size of rom, in 8Ko bank units.
    bank_nbr = (rom_size / 8192);

    // Get at least 8, 8ko bank to handle all 64ko Z80 memory space.
    if (bank_nbr < 8) bank_nbr = 8;
    bank16Ko_nbr = bank_nbr /2;

    cout << "Cartdrige contains " <<(int)bank16Ko_nbr << "*16Ko banks." << endl;

    // Allocate RAM for the whole cartridge.
    if (rom_size < 65536)
    {
        // Allocate at least 64Ko of ROM
	cartridge = new unsigned char[65536];
	for (int i = 0; i < 65536; i++)
	{
	    cartridge[i] = 0;
	}
    }
    else
    {
        cartridge = new unsigned char[rom_size];
    }

    if (cartridge == NULL)
    {
	cerr << "Unable allocate memory for cartridge."<< endl;
	cerr << "Exiting." << endl;
	exit(-1);
    }

    // Load rom file.
    ret = unzReadCurrentFile(myZip, cartridge, rom_size);
    if (ret <0)      // ret < 0 mean IO error.
    {
	cerr << "Unable to load cartridge from zip archive."<< endl;
	cerr << "Exiting." << endl;
	exit(-1);
    }
    unzClose(myZip);
    free (dummy);

	unsigned int crc = getCRC32(cartridge, rom_size);
	return crc;
}


/*------------------------------------------------------------*/
/* This method loads not zipped Rom into cartridge memory.    */
/*------------------------------------------------------------*/
unsigned int MemoryMapper::LoadSMSRom(const char *rom_file)
{
    /* Open ROM with file pointer at the end of ROM. */
    ifstream file(rom_file, ios::in | ios::binary | ios::ate);
    if (file.is_open() == false )
    {
        cerr << "Unable to open " << rom_file << " zip archive." << endl;
	exit (-1);
    }

    // Get the ROM size.
    rom_size = file.tellg();

    /* Some rom seems to have a 512byte header. Skip it. */
    if ((rom_size %1024) == 512)
    {
        cout << "512 bytes ROM header Skipped."<< endl;
  	file.seekg(512, ios::beg);
	rom_size -= 512;
    }
    else
    {
	file.seekg(0, ios::beg);
    }
    DisplayROMSize();

    // Save size of rom, in 8Ko bank units.
    bank_nbr = (rom_size / 8192);

    // Get at least 4 bank to handle all Z80 memory space.
    if (bank_nbr < 8) bank_nbr = 8;
    bank16Ko_nbr = bank_nbr /2;

    cout << "Cartdrige contains " <<(int)bank16Ko_nbr << "*16Ko banks." << endl;

    // Allocate RAM for the whole cartridge.
    if (rom_size < 65536)
    {
        // Allocate at least 64Ko of ROM
	cartridge = new unsigned char[65536];
	for (int i = 0; i < 65536; i++)
	{
	    cartridge[i] = 0;
	}
    }
    else
    {
        cartridge = new unsigned char[rom_size];
    }

    if (cartridge == NULL)
    {
	cerr << "Unable allocate memory for cartridge."<< endl;
	cerr << "Exiting." << endl;
	exit(-1);
    }

    // Now load the ROM.
    file.read((char*)cartridge, rom_size);
    if (file.good()== false)
    {
		cerr << "Unable to load .sms file !"<< endl;
		cerr << "Exiting." << endl;
		exit(-1);
    }
    file.close();

	unsigned int crc = getCRC32(cartridge, rom_size);
	return crc;
}

/*------------------------------------------------------------*/
/* This method displays roms size in mb or kb.		      */
/*------------------------------------------------------------*/
void MemoryMapper::DisplayROMSize()
{
    /* 128 is equivalent (romsize*8)/1024 */
    if ((rom_size/128)> 1024)
    {
	cout << "ROM size is " << rom_size << " bytes (" << ((rom_size*8)/(1024*1024))<< " mbits)." <<endl;
    }
    else
    {
        cout << "ROM size is " << rom_size << " bytes (" << ((rom_size * 8)/1024)<< " kbits)." <<endl;
    }
}

/*------------------------------------------------------------*/
/* This method return Rom name, based on Rom name, but with   */
/* no extension.					      */
/*------------------------------------------------------------*/
string MemoryMapper::getROMName()
{
    return rom_name;
}

void MemoryMapper::save_battery_backed_memory(string filename)
{
    // If bbr has been written:
    if (save_bbr)
    {
	ofstream file(filename.c_str(), ios::out | ios::binary);
	    if (file.is_open() == false )
	    {
            cerr << "Unable to create .bbr file !" << endl;
	    }

    	// Now save 2*16Ko banks.
	    file.write((const char*)sram,0x8000);
	    file.flush();
	    file.close();
		cout << "BBR has been saved." << endl;
    }
    else
    {
        cout << "BBR NOT saved." << endl;
    }
}

/*------------------------------------------------------------*/
/* This method computes CRC32 calculation on an uncompressed  */
/* ROM. It will be use for ROM that need special options, e.g */
/* -cm CodeMaster Mapper ect...								  */
/*------------------------------------------------------------*/
unsigned int MemoryMapper::getCRC32(unsigned char *buffer, unsigned int len)
{
    unsigned int crc32_;
    unsigned long crc_table[256];

    unsigned long c;
    unsigned int n, k;
   
    for (n = 0; n < 256; n++) 
	{
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) 
		{
        	if (c & 1) c = 0xedb88320L ^ (c >> 1);
         	else c = c >> 1;
        }
        crc_table[n] = c;
    }

	crc32_ = 0xFFFFFFFF;   
    for (n = 0; n < len; n++) 
	{
        crc32_ = crc_table[(crc32_ ^ buffer[n]) & 0xff] ^ (crc32_ >> 8);
    }

    return crc32_;
}

void MemoryMapper::setMapperType(Mapper m)
{
	switch (m)
	{
	    case SegaMapper:
			wr8_method = &MemoryMapper::wr8_sega_mapper;
		break;

	    case CodemasterMapper:
			wr8_method = &MemoryMapper::wr8_codemaster_mapper;
		break;

	    case KoreanMapper:
			wr8_method = &MemoryMapper::wr8_korean_mapper;
		break;
	}

	/* Keep a copy for save states. */
    mapperType = m;
}

/* Implemetntation of ImplementsSaveState. */
bool MemoryMapper::saveState( ofstream &ofs)
{
    MemoryMapperSaveState mss;

    mss.mapperType = mapperType;
    for (int i=0; i < 4; i++) mss.paging_regs[i] = paging_regs[i];
    
	/* Save 8Ko Ram from 0xC000-0xDFFF. */
	ofs.write((char *)&ram[0], 0x2000);
    if (!ofs.good()) return false;   

	/* Save 32Ko Battery Backed Memory. */
	ofs.write((char *)&sram[0], 0x8000);
    if (!ofs.good()) return false;  

	/* Save Paging Registers. */
	ofs.write((char *)&mss, sizeof(mss));
    if (!ofs.good()) return false; 
    return true;
}

bool MemoryMapper::loadState( ifstream &ifs)
{
    MemoryMapperSaveState mss;
	
	/* Load 8Ko Ram from 0xC000-0xDFFF. */
	ifs.read((char *)&ram[0], 0x2000);
    if (!ifs.good()) return false;   

	/* Load 32Ko Battery Backed Memory. */
	ifs.read((char *)&sram[0], 0x8000);
    if (!ifs.good()) return false;  

	/* Load Paging Registers. */
	ifs.read((char *)&mss, sizeof(mss));
    if (!ifs.good()) return false; 

    mapperType = mss.mapperType;
    for (int i=0; i < 4; i++) paging_regs[i] = mss.paging_regs[i];

	/* Force mapping registers to regenerate bank swapping. */
	switch (mapperType)
	{
	    case SegaMapper:
			wr8(0xFFFC, paging_regs[0]);
			wr8(0xFFFD, paging_regs[1]);
			wr8(0xFFFE, paging_regs[2]);
			wr8(0xFFFF, paging_regs[3]);
        break;

	    case CodemasterMapper:
			wr8(0x8000, paging_regs[0]);
        break;

	    case KoreanMapper:
			wr8(0xA000, paging_regs[0]);
        break;
	}
    return true;
}

