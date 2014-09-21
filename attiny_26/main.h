/*
 * main.h header file with various avr includes & defines for the main code
 */

// Standard AVR includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

//defines
#define NOTE_CTRL_DDRx DDRA
#define NOTE_CTRL_PORTx PORTA
#define NOTE_CTRL_PINx  PINA

#define NOTE_CHANNEL_SEL_BIT 7


