all: project.hex

project.elf: project.c game.h game.c
	avr-gcc -mmcu=atmega324a -Os -Wall -o project.elf project.c game.h game.c

project.hex: project.elf
	avr-objcopy  -j .text -j .data -O ihex project.elf project.hex
	avr-size project.elf

clean:
	rm -rf project.elf project.hex
