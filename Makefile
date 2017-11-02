# Name: Makefile
# Author: <insert your name here>

DEVICE     = atxmega8e5
CLOCK      = 2000000
PROGRAMMER = -p x8e5 -B32
OBJECTS    = main.o
FUSES      = -U fuse0:w:0xFF:m \
             -U fuse1:w:0x00:m \
             -U fuse2:w:0xBE:m \
             -U fuse4:w:0xFF:m \
             -U fuse4:w:0xEF:m \

INC_DIR = include
SRC_DIR = src
BLD_DIR = build

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I$(INC_DIR)

SRC_STRUCT := $(shell find $(SRC_DIR) -type d)
# Mirror the structure in the build dir
DIR_STRUCT := $(SRC_STRUCT:$(SRC_DIR)%=$(BLD_DIR)%)

# Collect source files
SRC    := $(addsuffix /*.c,$(SRC_STRUCT))
SRC    := $(wildcard $(SRC))
OBJ    := $(patsubst $(SRC_DIR)/%.c, $(BLD_DIR)/%.o, $(SRC))
#OBJ    := $(filter-out %/main.o, $(OBJ))

OBJECTS = $(OBJ)

#$(info $(OBJ))

# Make sure the folder structure exists in the build dir

# symbolic targets:
all:	$(BLD_DIR)/main.hex

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c | $(BLD_DIR)
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(BLD_DIR)/main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

clean:
	rm -rf $(BLD_DIR)

# Create the build dir
$(BLD_DIR):
	@$(foreach dir,$(DIR_STRUCT),mkdir -p $(dir);)

# file targets:
$(BLD_DIR)/main.elf: $(OBJECTS) | $(BLD_DIR)
	$(COMPILE) -o $(BLD_DIR)/main.elf $(OBJECTS)

$(BLD_DIR)/main.hex: $(BLD_DIR)/main.elf | $(BLD_DIR)
	@rm -f $(BLD_DIR)/main.hex
	avr-objcopy -j .text -j .data -O ihex $(BLD_DIR)/main.elf $(BLD_DIR)/main.hex
	avr-size --format=avr --mcu=$(DEVICE) $(BLD_DIR)/main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(BLD_DIR)/main.elf
	avr-objdump -d $(BLD_DIR)/main.elf

cpp:
	$(COMPILE) -E $(SRC_DIR)/main.c
