NAME = main
SERVER = server
MODULES = src/radio.o src/storage.o src/filesys.o src/map.o

CFLAGS = -I./include -g -Wall -Wpointer-arith
CFLAGS += -Og -std=c99 -ffreestanding
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name
LDFLAGS = -nostdlib -T memmap -L. -L./src -L./lib
LDLIBS  = -lpisd -lpiextra -lpi -lgcc

all: $(NAME).bin $(MODULES)

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@
	
%.elf: %.o $(MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(NAME).bin
	./util/rpi-install.py -p $<

server: $(SERVER).bin
	cp server.bin ./server/kernel.img

clean:
	rm -f *.o *.bin *.elf *.list *~
	rm -f src/*.o
	rm -f ./server/kernel.img

.PHONY: all clean install

.PRECIOUS: %.o %.elf

%:%.c
%:%.o