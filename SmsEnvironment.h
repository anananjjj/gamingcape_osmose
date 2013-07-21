#ifndef SMS_ENVIRONMENT_H
#define SMS_ENVIRONMENT_H

#include "Definitions.h"
#include <iostream>
#include "./cpu/Z80.h"
#include "MemoryMapper.h"
#include "IOMapper.h"
#include "IOMapper_GG.h"
#include "VDP.h"


using namespace std;

class SmsEnvironment : public Z80Environment
{
public:

    /** Constructor. */
    SmsEnvironment();

    /** Destructor. */
    virtual ~SmsEnvironment();
    virtual void disasm(u16 IP);
    virtual void onReturnFromInterrupt();
    virtual void onInterruptsEnabled();
    virtual void setMemoryMapper(MemoryMapper *m);
    virtual void setIOMapper(IOMapper *m);
    virtual void setVDP(VDP *v);
    virtual void setCPU(Z80 *c);

    // New core wrappers
    virtual u8 rd8( u16 addr );
    virtual void wr8( u16 addr, u8 value );
    virtual u8 in( u16 port );
    virtual void out( u16 port, u8 value );

    // Old Core Wrapper:
    virtual unsigned char readByte( unsigned addr ) {return rd8(addr);};
    virtual void writeByte( unsigned addr, unsigned char data ){wr8(addr, data);};
    virtual void writePort( unsigned port, unsigned char data ){out(port, data);};
    virtual unsigned char readPort( unsigned addr ){return in(addr);};

protected:

    MemoryMapper *mmapper;
    IOMapper *iomapper;
    VDP *v;
    Z80 *cpu;
};

#endif
