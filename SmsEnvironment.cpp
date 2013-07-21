#include <iostream>
#include <iomanip> // for trace in hexa in rw port
#include "./cpu/Z80.h"
#include "SmsEnvironment.h"

using namespace std;

/** Constructor. */
SmsEnvironment::SmsEnvironment() {
//    dasm = new Z80Dasm(*this);
}

/** Destructor. */
SmsEnvironment::~SmsEnvironment() {
}

void SmsEnvironment::disasm(u16 IP) {
    
    // This is done to avoid multiple disassembly of LDIR OTIR... instructions.
    static unsigned int last_pc = 0x10000; // impossible value in IP.
    //if (last_pc != IP)
    {
        //dasm->disasm(IP);
        last_pc = IP;
    }
}

/**
    Called immediately after a RETI is executed.
*/
void SmsEnvironment::onReturnFromInterrupt() 
{
}

void SmsEnvironment::onInterruptsEnabled() 
{
    if (v->irqAsserted())
    {
        cpu->interrupt(0xff);
    }
}

void SmsEnvironment::setMemoryMapper(MemoryMapper *m) {
    mmapper = m;
}

void SmsEnvironment::setIOMapper(IOMapper *m) {
    iomapper = m;
}

void SmsEnvironment::setVDP(VDP *vdp) {
    v = vdp;
}

void SmsEnvironment::setCPU(Z80 *c) {
    cpu = c;
}

/* 8 bits read operation. */
u8 SmsEnvironment::rd8( u16 addr ) 
{   
    //printf("r%.4x\n", addr);
    return mmapper->rd8(addr & 0xFFFF);
}

/* 8 bits write operation. */
void SmsEnvironment::wr8( u16 addr, u8 value )
{
    //printf("w%.4x, %.2x\n", addr, value);
    mmapper->wr8(addr & 0xFFFF, value);
}

/* 8 bits read IO operation. */
u8 SmsEnvironment::in( u16 port ) 
{   
    //printf("i%.4x\n", port & 0xff);
    return iomapper->in8(port & 0xff);
}

/* 8 bits write IO operation. */
void SmsEnvironment::out( u16 port, u8 value )
{
    //printf("o%.2x, %.2x\n", port & 0xff, value);
    iomapper->out8(port & 0xff,value);
}
