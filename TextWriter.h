/*****************************************************************************
*
* File: TextWriter.h
*
* Project: Osmose emulator.
*
* Description: This class is use to display textual information into
* a SDL_Surface.
*
* Author: Vedder Bruno
* Date: 26/02/2005, 14h13
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef TEXT_WRITER_H
#define TEXT_WRITER_H

#include <iomanip>
#include <SDL/SDL.h>

#define MESSAGE_LENGTH 31

class TextWriter
{
public:

    TextWriter();				// Constructor.
    void addText(const char *msg, unsigned int seconds); // Add message for n seconds.
    void update(SDL_Surface *s, bool);		// Update SDL_Surface.
    void writeText(SDL_Surface *s,char *str, unsigned int x, unsigned int y);
    unsigned int getDuration();

private:

    unsigned short fontCols[4];                 // 16bits cols for font color.
    char message[MESSAGE_LENGTH];
    bool hasMessage;
    unsigned int  messageDuration;
    void writeChar(SDL_Surface *s,unsigned char c, unsigned int x, unsigned int y);

    static const char asciiToFont[128];         // Font Lookup table.
    static const unsigned char font[8*496];     // Font Data.
};

#endif
