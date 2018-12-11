CLIENT = client
SERVER = server
MODULES = src/map.o src/storage.o src/filesys.o src/transmitter.o src/receiver.o src/wire.o src/socket.o src/compress.o

CFLAGS = -I./include -g -Wall -Wpointer-arith
CFLAGS += -O1 -std=c99 -ffreestanding
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name
LDFLAGS = -nostdlib -T memmap -L. -L./src -L./lib
LDLIBS  = -lpisd -lpiextra -lpi -lgcc

all: $(CLIENT).bin $(MODULES)

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

$(SERVER).elf: $(SERVER).o $(MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

$(CLIENT).elf: $(CLIENT).o $(MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

$(CLIENT).o: $(CLIENT).c
	arm-none-eabi-gcc -DPROG_ARG='"$(PROG_ARG)"' -DFN_ARG='"$(FN_ARG)"' -DDAT_ARG='"$(DAT_ARG)"' $(CFLAGS) -c $< -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(CLIENT).bin
	./util/rpi-install.py -p $<

server: $(SERVER).bin
	cp server.bin ./server/kernel.img
	rm /Volumes/SERVER\ SD/kernel.img
	cp server.bin /Volumes/SERVER\ SD/kernel.img

clean:
	rm -f *.o *.bin *.elf *.list *~
	rm -f src/*.o

transmit:
	@ make --silent clean
	@ make --silent PROG_ARG="$(Command)" FN_ARG="$(Filename)" DAT_ARG="$(Content)"
	@ make --silent install

.PHONY: all clean install

.PRECIOUS: %.o %.elf

%:%.c
%:%.o
