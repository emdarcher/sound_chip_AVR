

#include "main.h" //most #defines are in here
//#include "fullSine.h"
#include "notes.h"
#include "int8_sine.h"

/*
const int8_t fullSine[256] PROGMEM = {
  0, 3, 6, 9, 12, 15, 18, 21,
  24, 27, 30, 34, 37, 39, 42, 45,
  48, 51, 54, 57, 60, 62, 65, 68,
  70, 73, 75, 78, 80, 83, 85, 87,
  90, 92, 94, 96, 98, 100, 102, 104,
  106, 107, 109, 110, 112, 113, 115, 116,
  117, 118, 120, 121, 122, 122, 123, 124,
  125, 125, 126, 126, 126, 127, 127, 127,
  127, 127, 127, 127, 126, 126, 126, 125,
  125, 124, 123, 122, 122, 121, 120, 118,
  117, 116, 115, 113, 112, 110, 109, 107,
  106, 104, 102, 100, 98, 96, 94, 92,
  90, 87, 85, 83, 80, 78, 75, 73,
  70, 68, 65, 62, 60, 57, 54, 51,
  48, 45, 42, 39, 37, 34, 30, 27,
  24, 21, 18, 15, 12, 9, 6, 3,
  0, -4, -7, -10, -13, -16, -19, -22,
  -25, -28, -31, -35, -38, -40, -43, -46,
  -49, -52, -55, -58, -61, -63, -66, -69,
  -71, -74, -76, -79, -81, -84, -86, -88,
  -91, -93, -95, -97, -99, -101, -103, -105,
  -107, -108, -110, -111, -113, -114, -116, -117,
  -118, -119, -121, -122, -123, -123, -124, -125,
  -126, -126, -127, -127, -127, -128, -128, -128,
  -128, -128, -128, -128, -127, -127, -127, -126,
  -126, -125, -124, -123, -123, -122, -121, -119,
  -118, -117, -116, -114, -113, -111, -110, -108,
  -107, -105, -103, -101, -99, -97, -95, -93,
  -91, -88, -86, -84, -81, -79, -76, -74,
  -71, -69, -66, -63, -61, -58, -55, -52,
  -49, -46, -43, -40, -38, -35, -31, -28,
  -25, -22, -19, -16, -13, -10, -7, -4
};
*/

#define DO_YOU_WANT_SLEEP 0 //set to 1 if you want the cpu to sleep in loop
#define SET_SLEEP_ENABLE_BIT MCUCR|=(1<<SE)

//vars
volatile uint16_t accu_16bit;
volatile uint16_t inc_16bit;
volatile uint8_t note_port_store;

//prototypes
static inline void init_fast_pwm_timer1(void);
static inline void init_note_ctrl(void);

void main(void)
{
    
    init_note_ctrl();//init the note ctrl port
    init_fast_pwm_timer1(); //init the pll and timer stuff
    sei(); //enable global interrupts
    /*infinite loop */
    while(1){
    #if DO_YOU_WANT_SLEEP!=0
        SET_SLEEP_ENABLE_BIT;
        sleep_cpu();//go to sleep
    #endif
    }
}

static inline void init_fast_pwm_timer1(void){
    //based off init() in AVR131 document
    
    
    DDRB |= (1<<PB1); //set PB1 as output
    
    PLLCSR |= (1<<PLLE);//enable pll
    
    loop_until_bit_is_set(PLLCSR, PLOCK);//wait for the PLL to lock
    
    PLLCSR |= (1<<PCKE); //set pll as clock for peripherals (timer1 PCK)
    
    TCCR1A |= (1<<COM1A1);//set to clear on compare match, set when count is 1
    
    OCR1C = 0xFF; //set top count value to 255 (8-bits) total
    
    TCCR1A |= (1<<PWM1A); //enable pwm mode for OCR1A
    
    //TCCR1B |= (1<<CS11); //select PCK/2 for clock source (32MHz) 
    TCCR1B |= (1<<CS11)|(1<<CS10); //select PCK/4 (16MHz) source
    
    TIMSK |= (1<<TOIE1); //enable TOV1 interrupt (timer overflow)
    
}

static inline void init_note_ctrl(void){
    //inits the port for note control
    NOTE_CTRL_DDRx |= 0x00; //set all to input
}

//attiny26 isr for timer1 overflow
ISR(TIMER1_OVF1_vect){
//the interrupt routine
    uint8_t note_store; 
    note_port_store = (~NOTE_CTRL_PINx & ~(1<<NOTE_CHANNEL_SEL_BIT));
   /* 
    uint8_t temp_store = note_port_store;
    uint8_t j=71,i = 7;
    while(i--){
        note_port_store = (bit_is_set(temp_store,i)) ? j : note_port_store;
        --j;
    }
   */ 
   note_port_store += 60; // middle C for testing
    note_store = note_port_store; 
    note_store = (note_store >= 12) ? note_store : 12;
    accu_16bit += pgm_read_word(&inc16_note_vals[ MIDI_NUM_TO_INDEX(note_store) ]);
    
    OCR1A = 128 + pgm_read_byte(&int8_sine_table[ (uint8_t) (accu_16bit >> 8) ]);
}
