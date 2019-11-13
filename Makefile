
# toolchain
TOOLCHAIN    = arm-none-eabi-
CC           = $(TOOLCHAIN)gcc
CP           = $(TOOLCHAIN)objcopy
AS           = $(TOOLCHAIN)gcc -x assembler-with-cpp
HEX          = $(CP) -O ihex
BIN          = $(CP) -O binary -S

# define mcu, specify the target processor
MCU          = cortex-m4

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJECT_NAME=FreeRTOS

# specify define
DDEFS       =

# define root dir
ROOT_DIR     = .

# define include dir
INCLUDE_DIRS = $(ROOT_DIR)/Platform

# define stm32f4 lib dir
STM32F4_LIB_DIR      = $(ROOT_DIR)/Libraries

# define freertos dir
FREERTOS_DIR = $(ROOT_DIR)/Libraries/FreeRTOS

# define user dir
APP_DIR     = $(ROOT_DIR)/App

# link file
LINK_SCRIPT  = $(ROOT_DIR)/Platform/stm32f401cc_flash.ld

# user specific

ASM_SRC  = $(ROOT_DIR)/Platform/startup_stm32f401xx.s

SRC      =
SRC  	+= $(ROOT_DIR)/Platform/system_stm32f4xx.c
SRC  	+= $(ROOT_DIR)/Platform/stm32f4xx_it.c
SRC  	+= $(ROOT_DIR)/Platform/audio_codec.c
SRC     += $(APP_DIR)/main.c
SRC  	+= $(APP_DIR)/Usb/Audio/usbd_audio_core.c
SRC  	+= $(APP_DIR)/Usb/Audio/usbd_audio_out_if.c
SRC  	+= $(APP_DIR)/Usb/usbd_usr.c
SRC  	+= $(APP_DIR)/Usb/usbd_desc.c
SRC  	+= $(APP_DIR)/Usb/usb_bsp.c
SRC  	+= $(STM32F4_LIB_DIR)/syscall/syscalls.c

# user include
INCLUDE_DIRS  += $(APP_DIR)
INCLUDE_DIRS  += $(APP_DIR)/Usb
INCLUDE_DIRS  += $(APP_DIR)/Usb/Audio
INCLUDE_DIRS  += $(APP_DIR)/Hal

# include sub makefiles
include makefile_std_lib.mk   # STM32 Standard Peripheral Library
include makefile_freertos.mk  # freertos source

INC_DIR  = $(patsubst %, -I%, $(INCLUDE_DIRS))

# run from Flash
DEFS	 = $(DDEFS)

OBJECTS  = $(ASM_SRC:.s=.o) $(SRC:.c=.o)

# Define optimisation level here
OPT = -Os

MC_FLAGS = -mcpu=$(MCU) -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -finline-functions -Wdouble-promotion -std=gnu99

AS_FLAGS = $(MC_FLAGS) -lm -lc -lgcc -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst)
CP_FLAGS = $(MC_FLAGS) $(OPT) -fdata-sections -ffunction-sections -g -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
LD_FLAGS = $(MC_FLAGS) -u _scanf_float -u _printf_float -g -specs=nano.specs -specs=nosys.specs -gdwarf-2 -mthumb -nostartfiles -Xlinker -T$(LINK_SCRIPT) -Wl,-Map=$(PROJECT_NAME).map,--cref,--no-warn-mismatch,--gc-sections

#
# makefile rules
#
all: $(OBJECTS) $(PROJECT_NAME).elf  $(PROJECT_NAME).hex $(PROJECT_NAME).bin
	$(TOOLCHAIN)size $(PROJECT_NAME).elf

%.o: %.c Makefile makefile_std_lib.mk $(LINK_SCRIPT)
	@echo [CC] $(notdir $<)
	@$(CC) -c $(CP_FLAGS) -I . $(INC_DIR) $< -o $@

%.o: %.s
	@echo [AS] $(notdir $<)
	@$(AS) -c $(AS_FLAGS) $< -o $@

%.elf: $(OBJECTS)
	@echo [LD] $(PROJECT_NAME).elf
	@$(CC) $(OBJECTS) $(LD_FLAGS) -o $@

%.hex: %.elf
	@echo [HEX] $(PROJECT_NAME).hex
	@$(HEX) $< $@

%.bin: %.elf
	@echo [BIN] $(PROJECT_NAME).bin
	@$(BIN)  $< $@

clean:
	@echo [RM] OBJ
	@-rm -rf $(OBJECTS)
	@echo [RM] BIN "(ELF,HEX,BIN)"
	@-rm -rf $(PROJECT_NAME).elf
	@-rm -rf $(PROJECT_NAME).hex
	@-rm -rf $(PROJECT_NAME).bin
	@echo [RM] LST
	@-rm -rf $(SRC:.c=.lst)
	@-rm -rf $(ASM_SRC:.s=.lst)
	@echo [RM] MAP
	@-rm -rf $(PROJECT_NAME).map

