MCU := atmega328p
F_CPU := 16000000UL
PROG_TYPE := usbtiny

CC := avr-gcc
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
AVRSIZE := avr-size
AVRDUDE := avrdude

SRCS:=$(wildcard src/*.c)
OBJS:=$(SRCS:.c=.o)
DEPS:=$(SRCS:.c=.d)

TARGET = $(lastword $(subst /, ,$(CURDIR)))

CFLAGS = -DF_CPU=$(F_CPU) -Isrc/ -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -O2
LDFLAGS = -Wl,-Map,$(TARGET).map -Wl,--gc-sections
ARCHFLAGS = -mmcu=$(MCU)

AVRDUDE_FLAGS = -c$(PROG_TYPE) -p$(MCU)

.PHONY: all clean flash flash_eeprom flash_fuses
all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) $(ARCHFLAGS) -c -o $@ $<

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.eeprom: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%.lst: %.elf
	$(OBJDUMP) -S $< > $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(ARCHFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET).*

flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<

flash_eeprom: $(TARGET).eeprom
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U eeprom:w:$<

flash_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(FUSE_STRING)

-include $(DEPS)
