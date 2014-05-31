# File: makefile
# Author: Jarielle Catbagan
# Created: 29, May 2014

build: programmer.o
	avr-gcc -mmcu=atmega328p -o programmer.elf programmer.o
	avr-objcopy -j .text -j .data -O ihex programmer.elf programmer.hex
programmer.o: programmer.c
	avr-gcc -Os -mmcu=atmega328p -std=c99 -c programmer.c
mem.o: mem.c
	avr-gcc -Os -mmcu=atmega328p -c mem.c
sst.o: sst.c
	avr-gcc -Os -mmcu=atmega328p -c sst.c
shiftreg.o: shiftreg.c
	avr-gcc -Os -mmcu=atmega328p -c shiftreg.c
program: programmer.hex
	avrdude -p atmega328p -P usb -c usbtiny -U flash:w:programmer.hex
clean: 
	rm -f programmer.o
	rm -f mem.o
	rm -f sst.o
	rm -f shiftreg.o
	rm -f programmer.elf
	rm -f programmer.hex
