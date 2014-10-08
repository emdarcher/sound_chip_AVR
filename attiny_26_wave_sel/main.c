

#include "main.h" //most #defines are in here
//#include "fullSine.h"
#include "notes.h"
#include "int8_sine.h"

#define DO_YOU_WANT_SLEEP 1 //set to 1 if you want the cpu to sleep in loop
#define SET_SLEEP_ENABLE_BIT MCUCR|=(1<<SE)

//vars
volatile uint16_t accu_16bit;
//volatile uint16_t inc_16bit;
//volatile uint8_t note_port_store;

//prototypes
static inline void init_fast_pwm_timer1(void);
static inline void init_note_ctrl(void);
static inline void init_wave_ctrl(void);

void main(void)
{
        
    init_note_ctrl();//init the note ctrl port
    init_wave_ctrl();//init the wave control pins
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
    NOTE_CTRL_DDRx &= 0x00; //set all to input
}


static inline void init_wave_ctrl(void){
    //inits the port and pins for wave ctrl
    //set to input
    WAVE_CTRL_DDRx &= ~((1<<WAVE_CTRL_BIT0)|(1<<WAVE_CTRL_BIT1));
    
}

//attiny26 isr for timer1 overflow
ISR(TIMER1_OVF1_vect){
//the interrupt routine
    uint8_t note_port_store;
    uint8_t note_store; 
    uint8_t wave_bits_store;
    uint8_t temp_OCregA;
    note_port_store = (~NOTE_CTRL_PINx & ~(1<<NOTE_CHANNEL_SEL_BIT));
    wave_bits_store = ((WAVE_CTRL_PINx & ((1<<WAVE_CTRL_BIT1)|(1<<WAVE_CTRL_BIT0)))>>WAVE_CTRL_BIT0);
    //note_port_store = (NOTE_CTRL_PINx & ~(1<<NOTE_CHANNEL_SEL_BIT));
    //note_port_store = 60; // middle C for testing
   
    note_store = (note_port_store>0) ? (note_port_store-1) : 0; 
    //note_store = 60;
    note_store = (note_store >= 12) ? note_store : 12;
    accu_16bit += pgm_read_word(&inc16_note_vals[ MIDI_NUM_TO_INDEX(note_store) ]);
    uint8_t accu_8bit = (uint8_t)(accu_16bit >> 8);
    switch (wave_bits_store) {
        case WAVE_SINE:
            temp_OCregA = 128 + pgm_read_byte(&int8_sine_table[ accu_8bit  ]); 
            break;
        case WAVE_SQUARE:
            //temp_OCregA = ((accu_8bit)<128) ? 255 : 0; //if less than 128, then set 255 (HIGH), else 0 (LOW)

            /* all of the three code lines below will work and are likely to compile the same,
            (I've checked in the dissassebled code for each of them) uncomment the one you want to use */
            //temp_OCregA = ((accu_8bit)>>7) ? 0 : 255;
            temp_OCregA = ((accu_8bit)>127) ? 0:255; //same as below but easier to read and compiles the same.
            //temp_OCregA = ((accu_8bit)&(1<<7)) ? 0:255; //set to 0 if bit 7 is set (value >=128), else 255.
            break;
        case WAVE_TRIANGLE:
            temp_OCregA = ((accu_8bit)&(1<<7)) ? 255-((accu_8bit & ~(1<<7))<<1) : (accu_8bit<<1);
            break;
        case WAVE_SAW:
            temp_OCregA = accu_8bit;
            break;
    }
    OCR1A = temp_OCregA;
    //OCR1A = 128 + pgm_read_byte(&int8_sine_table[ (uint8_t) (accu_16bit >> 8) ]);
}
