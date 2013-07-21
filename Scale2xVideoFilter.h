#ifndef SCALE2X_H
#define SCALE2X_H

#include "VideoFilter.h"

class Scale2xVideoFilter : public VideoFilter {
public:    

    int getFinalOutputWidth();
    int getFinalOutputHeight();
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();

private:
    void FilterTopBorder(unsigned short *s,unsigned short *d);
    void FilterBottomBorder(unsigned short *s,unsigned short *d);
    void FilterLeftBorder(unsigned short *s,unsigned short *d);
    void FilterRighttBorder(unsigned short *s,unsigned short *d);

};

#endif
