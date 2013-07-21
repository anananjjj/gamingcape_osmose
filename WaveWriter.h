/*****************************************************************************
*
* File: waveWriter.h
*
* Project: Osmose emulator.
*
* Description: This class declares waveWriter class used by Osmose for
* saving sound. It's only designed to handle 16 bits mono 22050hz samples.
*
* Author: Vedder Bruno
* Date: 25/11/2004, 14h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef WAVE_WRITER_H
#define WAVE_WRITER_H

#include <iostream>
#include <fstream>
#include <iomanip>

#define WAVE_BUFFER_SIZE	5*22050	// 5 second buffer.

using namespace std;

typedef struct 
{
    char           chunkID[4];
    long           chunkSize;
    short          wFormatTag;
    unsigned short wChannels;
    unsigned long  dwSamplesPerSec;
    unsigned long  dwAvgBytesPerSec;
    unsigned short wBlockAlign;
    unsigned short wBitsPerSample;
} FormatChunk;

typedef struct 
{
    char           chunkID[4];
    long           chunkSize;
    short         *waveformData;
} DataChunk;

class WaveWriter {

public:
    unsigned int index;
    FormatChunk format;
    DataChunk   data;
    WaveWriter(const char *filename);	// Constructor.
    ~WaveWriter();			// Destructor.
    void writeData(short d);
    void close();
     
private:

    unsigned int length;
    ofstream waveFile;
    bool skip_save;
};

#endif
