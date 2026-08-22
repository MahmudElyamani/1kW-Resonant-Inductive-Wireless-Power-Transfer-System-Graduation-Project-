#
# Makefile - bare-metal STM32F103C6 4-channel 85kHz PWM project
# Requires the GNU Arm Embedded Toolchain (arm-none-eabi-gcc).
#

TARGET      = pwm_app
BUILD_DIR   = build

CC          = arm-none-eabi-gcc
OBJCOPY     = arm-none-eabi-objcopy
SIZE        = arm-none-eabi-size

MCU_FLAGS   = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft

C_SOURCES   = src/main.c \
              src/clock.c \
              src/gpio.c \
              src/timer.c \
              src/adc.c \
              src/watchdog.c \
              src/interrupt.c

ASM_SOURCES = startup/startup_stm32f103c6.s

INCLUDES    = -Iinc

CFLAGS      = $(MCU_FLAGS) $(INCLUDES) -Wall -Wextra -Os \
              -ffunction-sections -fdata-sections -std=c11 -g
ASFLAGS     = $(MCU_FLAGS) -g
LDFLAGS     = $(MCU_FLAGS) -Tlinker/stm32f103c6.ld -nostartfiles \
              -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

C_OBJECTS   = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
ASM_OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))

vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(ASM_OBJECTS) $(C_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

.PHONY: all clean flash
