#ifndef VIDEO_FILTER_LIST_H
#define VIDEO_FILTER_LIST_H

#include "NullVideoFilter.h"
#include "TvVideoFilter.h"
#include "MonochromVideoFilter.h"
#include "Scale2xVideoFilter.h"
#include "BilinearVideoFilter.h"
#include "Scale2xScanlineVideoFilter.h"
#include "Nearest2xVideoFilter.h"

#define NULL_FILTER              0
#define TV_FILTER                1
#define MONOCHROM_FILTER         2
#define SCALE2X_FILTER           3
#define BILINEAR_FILTER          4
#define SCALE2XSCANLINE_FILTER   5
#define NN2X_FILTER              6

#endif
