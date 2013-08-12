#
# GHOLUD
#

BUILD_DIR = build

# MCU name
MCU = atmega128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
F_CPU = 3686400

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s


# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS =

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

#-------------------------- Compiler Options C ---------------------------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#CFLAGS = -g$(DEBUG)

# Use one object file of whole project for optimization
#CFLAGS += --combine
#CFLAGS += -fwhole-program

CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -O$(OPT)
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -mno-interrupts
CFLAGS += -mcall-prologues
#CFLAGS += -mno-tablejump
#CFLAGS += -finline-functions

# Don't add not used functions to the binary
CFLAGS += -ffunction-sections

# Don't add not used data to the binary
CFLAGS += -fdata-sections

CFLAGS += $(CSTANDARD)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += $(CDEFS)
CFLAGS += -mmcu=$(MCU)

#CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
#CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
#CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

#------------------------------- Library Options -------------------------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB =
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB =
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS =

#------------------------ External Memory Options ------------------------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

#--------------------------- Linker Options ------------------------------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,--gc-sections
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
#LDFLAGS += -T linker_script.x

#------------------------ Programming Options (avrdude) ------------------------

# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
AVRDUDE_PROGRAMMER = usbasp

# com1 = serial port. Use lpt1 to connect to parallel port.
# AVRDUDE_PORT = com1    # programmer connected to serial device

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep


# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude>
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)



SHELL = sh
#CC = avr-gcc
CC = avr-gcc-4.7.2
LD = avr-ld

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf


# Target file name (without extension).
TARGET = patima

# Create object files directory

all: build_dir gccversion $(BUILD_DIR)/$(TARGET).elf\
	$(BUILD_DIR)/$(TARGET).hex size

build_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/drivers

program: $(BUILD_DIR)/$(TARGET).hex
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

# Display compiler version information.
gccversion :
	@echo
	@echo "		  --- $(TARGET) compilation ---"
	@echo
	@$(CC) --version

DRIVERS_SOURCES =\
	drivers/bc1602e.c\
	drivers/ds1631.c\
	drivers/ds18b20.c\
	drivers/fm24c64.c\
	drivers/pcf8583.c

# Make automatic rules to get all objects files from all files in catalogs,
# including even files that is not in given object list made above.
${BUILD_DIR}/drivers/%.o: drivers/%.c
	${CC} -c ${CFLAGS} -o ${@} ${<}

ROOT_SOURCES =\
	patima.c\
	alm.c\
	arr.c\
	bcd.c\
	cint.c\
	clc.c\
	crc.c\
	ctmp.c\
	def.c\
	delay.c\
	dmem.c\
	dsp.c\
	err.c\
	iic.c\
	ini.c\
	kbd.c\
	light.c\
	mat.c\
	mctr.c\
	mem.c\
	mem_fm24c64.c\
	mexe.c\
	mopr.c\
	msg.c\
	note.c\
	prm.c\
	ptimer.c\
	rmd.c\
	rtc.c\
	sgn.c\
	sta.c\
	stm.c\
	stp.c\
	stw.c\
	tc0.c\
	tc1.c\
	tc3.c\
	tdf.c\
	tmp.c\
	tmr.c\
	trg.c\
	usr.c\
	wdt.c\
	wire.c\
	wrt.c\
	zmr.c

ALL_SOURCES = ${DRIVERS_SOURCES} ${ROOT_SOURCES}


# Make automatic rules to get all objects files from all files in catalogs,
# including even files that is not in given object list made above.
# Option -MD needed to generate .d files in order to include them.
${BUILD_DIR}/%.o: %.c
	${CC} ${CFLAGS} -c -MD ${<} -o ${@}

# Make full list of object files, each name is unique and consist from catalog
# and base name of c file.
OBJECTS := $(ALL_SOURCES:.c=.o)
OBJECTS := $(addprefix ${BUILD_DIR}/, ${OBJECTS})

# Compile all project from list of object files in build dir
$(BUILD_DIR)/$(TARGET).elf : $(OBJECTS)
	${CC} ${CFLAGS} -o ${@} ${^} ${LDFLAGS}

# Convert elf to hex
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -R .eeprom -O $(FORMAT) $+ "$(BUILD_DIR)/$(TARGET).hex"

size: $(BUILD_DIR)/$(TARGET).elf
	@$(SIZE) --mcu=$(MCU) --format=avr $+

#include targets for all headers, generated by -MD command
include $(wildcard $(BUILD_DIR)/*.d)

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -rf cscope*

.PHONY: clean distclean
