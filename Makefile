#Zlib
Z_LIB_DIR=  ./zlib/
Z_LIB    =  $(Z_LIB_DIR)libz.a

#unzip based on zlib.
UNZIP_DIR=  ./unzip/
UNZIP_LIB=  $(UNZIP_DIR)unzip.a

DISASM_DIR = ./z80_disasm

# Uncomment these lines before builting with internal debugger.
#OPT_OBJS   = $(DISASM_DIR)/Z80Dasm.o \
#			SmsDebugger.o

OSM_OBJS =      main.o \
		./cpu/Z80.o \
		./cpu/Opc_cbxx.o \
		./cpu/Opc_dd.o \
		./cpu/Opc_ddcb.o \
		./cpu/Opc_ed.o \
		./cpu/Opc_fd.o \
		./cpu/Opc_fdcb.o \
		./cpu/Opc_std.o \
		OsmoseCore.o\
       	IOMapper.o \
		IOMapper_GG.o \
       	VDP.o \
		VDP_GG.o \
       	MemoryMapper.o \
       	SmsEnvironment.o \
       	SN76489.o \
       	WaveWriter.o \
		Options.o \
		OsmoseConfiguration.o \
		KeyConversion.o \
		NullVideoFilter.o \
		TvVideoFilter.o \
		MonochromVideoFilter.o \
		Scale2xVideoFilter.o \
		BilinearVideoFilter.o \
		Nearest2xVideoFilter.o \
		Scale2xScanlineVideoFilter.o \
		TextWriter.o \
		PadInputDevice.o \
		PaddleInputDevice.o \
		JoystickInputDevice.o \
		FIFOSoundBuffer.o \
		DebugEventThrower.o \
		RomSpecificOption.o \
		beagleboy.o

OSM_EXE         =  osmose

# Compiler/Linker Flags
# add -pg on LFLAGS and CFLAGS for gprof
RPIOPT =  -Ofast -ffast-math -fstrict-aliasing -fomit-frame-pointer -funsafe-math-optimizations
CFLAGS =  -Wall -D__USE_UNIX98 $(RPIOPT)
LFLAGS =  -lSDL
AFLAGS =  -f elf
CC  = g++
C_COMP = gcc
ASM = nasm

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

all:  libs disasm osmose stripexe

osmose: $(z80_lib) $(OSM_OBJS) $(OPT_OBJS)
	$(CC)  $(CFLAGS) $(OSM_OBJS) $(Z80_LIB) $(OPT_OBJS) $(UNZIP_LIB) $(Z_LIB) -o $(OSM_EXE) $(LFLAGS)

disasm: $(DISASM_OBJS)

stripexe:
	strip -s $(OSM_EXE)

clean:
	rm -f $(DISASM_DIR)/*.o
	rm -f ./cpu/*.o
	rm -f *.o core $(OSM_EXE)
	rm -f $(Z_LIB_DIR)*.o $(Z_LIB_DIR)*.a
	rm -f $(UNZIP_DIR)*.o $(UNZIP_DIR)*.a

# Clear all objects except ./cpu and zlib.
# This is done because compilation of CPU is long.
miniclean:
	rm -f *.o core $(OSM_EXE)

libs:
	cd $(Z_LIB_DIR) && make libz.a
	cd $(UNZIP_DIR) && make unzip.a





