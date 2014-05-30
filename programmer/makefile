# File: makefile
# Author: Jarielle Catbagan
# Created: 29, May 2014

build: programmer.o
	avr-gcc -mmcu=atmega328p -o programmer.elf programmer.o
	avr-objcopy -j .text -j .data -O ihex programmer.elf programmer.hex
programmer.o: programmer.c
	avr-gcc -Os -mmcu=atmega328p -std=c99 -c programmer.c
program: programmer.hex
	avrdude -p atmega328p -P usb -c usbtiny -U flash:w:programmer.hex
clean: 
	rm -f programmer.o
	rm -f programmer.elf
	rm -f programmer.hex
