# Tiva Makefile
# #####################################
#
# Part of the uCtools project
# uctools.github.com
#
#######################################
# user configuration:
#######################################

# Path to the ARM cross-compiler:
# Linux users: the path will be $(HOME)/Embedded/gcc-arm-none-eabi-5_4-2016q3/bin
# Windows users: the path will be "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin"
#XC_PATH = $(HOME)/Embedded/gcc-arm-none-eabi-5_4-2016q3/bin
XC_PATH = /opt/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major

# Serial port:
# Linux users: the port will be something like /dev/ttyACM0
# Windows users: the port will be something like COM4
PORT=/dev/ttyACM0

# Terminal emulator:
# Linux: the terminal emulator is screen
# Windows: the terminal emulator is putty (all lowercase)
TERMEMU=screen

# TIVAWARE_PATH: path to tivaware folder
TIVAWARE_PATH = $(HOME)/Embedded/tivaware

# FLASH_PATH: path to lm4flash (Linux) or LM Flash Programmer (Windows)
# Linux: $(HOME)/Embedded/lm4tools/lm4flash
# Windows: "C:/Program Files (x86)/Texas Instruments/Stellaris/LM Flash Programmer"
FLASH_PATH = $(HOME)/Embedded/lm4tools/lm4flash

# additional libraries
# libdriver.a path: tivaware/driverlib/gcc/
DRIVERLIB_PATH = $(TIVAWARE_PATH)/driverlib/gcc

# TARGET: name of the output file
TARGET = main
# MCU: part number to build for
MCU = TM4C123GH6PM
# SOURCES: list of input source sources
SRCDIR = src
INCDIR = inc
SOURCES := $(wildcard $(SRCDIR)/*.c)
# INCLUDES: list of includes, by default, use Includes directory
# INCLUDES := $(wildcard $(SRCDIR)/*.h)
# OUTDIR: directory to use for output
OUTDIR = build


LDLIBS = -L$(DRIVERLIB_PATH) -ldriver -lc -lm

# LD_SCRIPT: linker script
LD_SCRIPT = $(MCU).lds

# define flags
CFLAGS = -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DPART_$(MCU) -I$(TIVAWARE_PATH) -I$(INCDIR) -I$(XC_PATH)/include
CFLAGS += -Og -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall
CFLAGS += -specs="nosys.specs"
LDFLAGS = -DPART_$(MCU) -Og -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf
LDFLAGS += -Wall -specs="nosys.specs" -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -Wl,-Map=$(TARGET).map -T$(LD_SCRIPT)
#LDFLAGS = -Map=$(TARGET).map --entry ResetISR --gc-sections -T$(LD_SCRIPT)

#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
CC = $(XC_PATH)/bin/arm-none-eabi-gcc
#LD = $(XC_PATH)/arm-none-eabi-ld
LD = $(XC_PATH)/bin/arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -f
#if on windows use a different RM
ifdef ComSpec
    RM = del /Q
endif
MKDIR	= mkdir -p
#######################################

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))

# default: build bin
all: $(OUTDIR)/$(TARGET).bin

$(OUTDIR)/%.o: src/%.c | $(OUTDIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OUTDIR)/a.out: $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) -Wl,--start-group $(LDLIBS) -Wl,--end-group

$(OUTDIR)/main.bin: $(OUTDIR)/a.out
	$(OBJCOPY) -O binary $< $@
	mv $(TARGET).map $(OUTDIR)

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

flash:
	$(FLASH_PATH)/lm4flash $(shell pwd)/$(OUTDIR)/main.bin

clean:
	-$(RM) $(OUTDIR)/*

.PHONY: all clean

.PHONY: print_vars
print_vars:
	@echo "XC_PATH:" $(XC_PATH)
	@echo "PORT:" $(PORT)
	@echo "TERMEMU:" $(TERMEMU)
	@echo "SRCDIR:" $(SRCDIR)
	@echo "SOURCES:" $(SOURCES)
	@echo "INCLUDES:" $(INCDIR)
	@echo "TIVAWARE_PATH:" $(TIVAWARE_PATH)
	@echo "LDLIBS:" $(LDLIBS)
	@echo "OUTDIR:" $(OUTDIR)

.PHONY: putty
putty :
	$(TERMEMU) -serial -sercfg 115200 $(PORT)

.PHONY: screen
screen :
	$(TERMEMU) $(PORT) 115200

