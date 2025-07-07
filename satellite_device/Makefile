
######################################
# target
######################################
TARGET = SATELLITE_AT32F435ZMT7


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES = \
src/main.c \
src/conf/clock_nvic_perif_initialization/at32f435_437_wk_config.c \
src/conf/interrupts_handlers/at32f435_437_int.c \
src/conf/timebase/wk_system.c \
src/drivers/wk_acc.c \
src/drivers/wk_usb_otgfs.c \
src/drivers/led.c \
src/drivers/button.c \
src/core/usb_app/usb_app.c \
src/core/usb_descriptor/custom_hid_desc.c \
src/core/usb_hit_class/custom_hid_class.c \
src/core/controlled_systems/controlled_systems.c \
libraries/cmsis/cm4/device_support/system_at32f435_437.c \
libraries/drivers/src/at32f435_437_crm.c \
libraries/drivers/src/at32f435_437_tmr.c \
libraries/drivers/src/at32f435_437_ertc.c \
libraries/drivers/src/at32f435_437_gpio.c \
libraries/drivers/src/at32f435_437_i2c.c \
libraries/drivers/src/at32f435_437_usart.c \
libraries/drivers/src/at32f435_437_pwc.c \
libraries/drivers/src/at32f435_437_adc.c \
libraries/drivers/src/at32f435_437_spi.c \
libraries/drivers/src/at32f435_437_dma.c \
libraries/drivers/src/at32f435_437_edma.c \
libraries/drivers/src/at32f435_437_debug.c \
libraries/drivers/src/at32f435_437_flash.c \
libraries/drivers/src/at32f435_437_crc.c \
libraries/drivers/src/at32f435_437_wwdt.c \
libraries/drivers/src/at32f435_437_wdt.c \
libraries/drivers/src/at32f435_437_exint.c \
libraries/drivers/src/at32f435_437_misc.c \
libraries/drivers/src/at32f435_437_scfg.c \
libraries/drivers/src/at32f435_437_usb.c \
libraries/drivers/src/at32f435_437_acc.c \
libraries/drivers/src/at32f435_437_can.c \
libraries/drivers/src/at32f435_437_dac.c \
libraries/drivers/src/at32f435_437_xmc.c \
libraries/drivers/src/at32f435_437_qspi.c \
libraries/drivers/src/at32f435_437_sdio.c \
libraries/drivers/src/at32f435_437_dvp.c \
libraries/drivers/src/at32f435_437_emac.c \
libraries/middlewares/usb_drivers/src/usb_core.c \
libraries/middlewares/usb_drivers/src/usbd_core.c \
libraries/middlewares/usb_drivers/src/usbd_int.c \
libraries/middlewares/usb_drivers/src/usbd_sdr.c \
libraries/middlewares/freertos/source/croutine.c \
libraries/middlewares/freertos/source/event_groups.c \
libraries/middlewares/freertos/source/list.c \
libraries/middlewares/freertos/source/queue.c \
libraries/middlewares/freertos/source/stream_buffer.c \
libraries/middlewares/freertos/source/tasks.c \
libraries/middlewares/freertos/source/timers.c \
libraries/middlewares/freertos/source/portable/GCC/ARM_CM4F/port.c \
libraries/middlewares/freertos/source/portable/memmang/heap_4.c \

# ASM sources
ASM_SOURCES = \
libraries/cmsis/cm4/device_support/startup/gcc/startup_at32f435_437.s

# ASM sources
ASMM_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DAT32F435ZMT7


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES = \
-Ilibraries/drivers/inc \
-Ilibraries/cmsis/cm4/core_support \
-Ilibraries/cmsis/cm4/device_support \
-Ilibraries/middlewares/usb_drivers/inc \
-Ilibraries/middlewares/freertos/source/include \
-Ilibraries/middlewares/freertos/source/portable/GCC/ARM_CM4F \
-Isrc/conf \
-Isrc/conf/clock_nvic_perif_initialization \
-Isrc/conf/interrupts_handlers \
-Isrc/conf/timebase \
-Isrc/drivers \
-Isrc/core/usb_app \
-Isrc/core/usb_descriptor \
-Isrc/core/usb_hit_class \
-Isrc/core/controlled_systems \


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ld/AT32F435xM_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	@echo

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	@echo
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	@echo

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo
	$(SZ) $@
	@echo

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	@echo
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	@echo
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)