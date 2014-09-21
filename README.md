sound_chip_AVR
==============

Using the High Speed or regular fast PWM modes of various AVR MCU's to make
sounds and tones using DDS (Direct Digital Synthesis).

On ATtiny26: (WIP!)
-----------
The attiny26 code takes a MIDI note number (0-127, 7-bit) from a read of the first 7 bits of PORTA in parallel, then outputs that particular note using a PWM channel and DDS for Sine Wave generation ( through a Low-Pass filter ). This may expand to use OC1A and OC1B to have two channels, and you select one or the other using the 8th bit of PORTA. I may add (if there is space, and spare ISR cycles) the ability to select from Square, Triange, Saw, or Sine waves for the output by using a 2-bit (0-3) control on 2 of the pins on PORTB.

Other plans:
----------
  * Make code to run on an ATtiny85 that has control over I2C/TWI or SPI if possible, and maybe 2 channels.

