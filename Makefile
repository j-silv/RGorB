####################################################################################################
# Make configuration to build STM32 using bare-metal with CMSIS
# 
# Depends on the following directory structure:
#
# ├── build      -> contains generated output files such as firmware image and object code
# ├── config	 -> contains linker script and startup code
# ├── docs       -> contains documentation about this project
# ├── include	 -> contains all header files including CMSIS code
# ├── src		 -> contains all source code including main.c
# ├── Makefile	 -> this file
# └── README.md  -> explains a little about the project
#
# Makefile Inspired from:
# https://makefiletutorial.com/#makefile-cookbook
# https://github.com/cpq/bare-metal-programming-guide?tab=readme-ov-file#makefile-build-automation
# https://vivonomicon.com/2018/04/02/bare-metal-stm32-programming-part-1-hello-arm/
#
# Note I am not so happy with this format because we have assembly files
# and also .c files. This makefile will need to evolve somewhat in the future, but for now
# it's fine I suppose.
####################################################################################################

.EXTRA_PREREQS := $(abspath $(lastword $(MAKEFILE_LIST))) # so that we recompile when this makefile changes
BUILD_DIR = ./build
SRC_DIRS = ./src ./config
INC_DIRS = ./include
TARGET_LINK = firmware.elf
TARGET_FLASH = firmware.bin
LINKER = ./config/STM32F446RETX_FLASH.ld
TOOL_PATH = ~/files/coding/tools/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/
CC = ${TOOL_PATH}arm-none-eabi-gcc
OC = ${TOOL_PATH}arm-none-eabi-objcopy
NM = ${TOOL_PATH}arm-none-eabi-nm
DB = ${TOOL_PATH}arm-none-eabi-gdb
SZ = ${TOOL_PATH}arm-none-eabi-size
FLASHER = /usr/bin/st-flash
ARCH = cortex-m4
#SRC_IGNORE = ./src/system_stm32f4xx.c

####################################################################################################

INC_DIRS := $(shell find $(INC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')
SRCS := $(subst $(SRC_IGNORE),,$(SRCS))

OBJS := $(addsuffix .o,$(basename $(SRCS)))
OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(OBJS)))
DEPS := $(OBJS:.o=.d) # Dependency files for headers

# by default, -g is actually -g2, so we can't get macro processing defintions
CFLAGS ?= -c -O0 -mcpu=$(ARCH) -mthumb -Wall -DSTM32F446xx -g3 -MMD -MP
LDFLAGS ?= -T$(LINKER) -mcpu=$(ARCH) -mthumb -Wall -Wl,-Map=$@.map -specs=nosys.specs -Wl,--gc-sections  

####################################################################################################

all: $(BUILD_DIR)/$(TARGET_FLASH)

flash: $(BUILD_DIR)/$(TARGET_FLASH)
	$(FLASHER) --reset write $< 0x08000000

link: $(BUILD_DIR)/$(TARGET_LINK)

build: $(OBJS)

debug:
# st-util
# arm-none-eabi-gdb ./build/firmware.elf
# target extended-remote :4242
# load
# continue

clean:
	rm -rf $(BUILD_DIR)

####################################################################################################

$(BUILD_DIR)/$(TARGET_FLASH): $(BUILD_DIR)/$(TARGET_LINK)
	$(OC) -O binary $< $@

$(BUILD_DIR)/$(TARGET_LINK): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	$(NM) $@ -S -n -l > $@.nm
	$(SZ) $@ > $@.size
	

$(BUILD_DIR)/%.o: ./src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: ./config/%.s
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

####################################################################################################

# Include dependency files if they exist
-include $(DEPS)

.PHONY: all build link flash clean