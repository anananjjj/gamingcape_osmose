/*****************************************************************************
*
* File: SN76489.h
*
* Project: OldSpark emulator.
*
* Description: This class will define SN76489 interface.
*
* Author: Vedder Bruno
* Date: 18/11/2004, 18h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef SN76489_H
#define SN76489_H

#include "FIFOSoundBuffer.h"
#include "SaveState.h"

using namespace std;

#define NOISE_INITIAL_STATE 0x4000
#define WHITE_NOISE_FEEDBACK 0xF037
#define PERIODIC_NOISE_FEEDBACK 0x8000

/* #define PSG_DEBUG */
/* Hardware clock. */
enum
{
    sn76489_max_clock = 4000000, //  4.0 Mhz
    sn76489_min_clock = 500000,  //  500 khz
};

typedef struct
{

    u8  lastRegister_;     // Last written register
    u8  volume_[4];	       // 4 bits volume registers.
    u16 freqDiv_[4];       // Frequence divider / 10 bits Tone registers.
    u32 half_period_[4];
    u32 period_counter_[4];
    bool  whiteNoise_;			// WhiteNoise/Periodic mode flag.
    bool follow_tone2_frequency_;
    u16 LFSR_;

} SN76489SaveState;

class SN76489 : public ImplementsSaveState
{

public:

    /* Constructor. */
    SN76489(u32 chip_frequency, u32 samplingRate);

    /* Destructor. */
    ~SN76489(){ delete Fifo_; }

    /* Handle port writing. */
    void writePort(u8);

    /* Write wave to audio buffer.  */
    void getWave(u8 *, s32);

    /* run for N cycles. */
    bool run(u32);

    /* Method used by WaveWriter to save samples.*/
    s16 getLastSample() {return last_sample_;}

    /* Reset PSG.  */
    void reset();

    /* Get FIFO Object for mixer for example.*/
    FIFOSoundBuffer *getFIFOSoundBuffer() { return Fifo_;}

	/* Implemetntation of ImplementsSaveState. */
    bool saveState( ofstream &ofs);
    bool loadState( ifstream &ifs);

private:
    FIFOSoundBuffer *Fifo_;
    u32 chip_frequency_;
    u32 clock_;            // Chip Frequency / 16
    u8  lastRegister_;     // Last written register
    u8  volume_[4];	       // 4 bits volume registers.
    u16 freqDiv_[4];       // Frequence divider / 10 bits Tone registers.

    u32 half_period_[4];
    u32 period_counter_[4];
    u16 channel_output_[4];
    u32 update_step_;
    s16 last_sample_;
    bool  whiteNoise_;			// WhiteNoise/Periodic mode flag.
    u16 LFSR_;
    static const u16 volume_table[16];
    bool follow_tone2_frequency_;

    /* Internal utility method to compute parity.*/
    u8   parity(u16 v);
};
#endif
