//notes and stuff


#define PWM_CLK_HZ 16000000UL //THE PWM CLOCK SPEED AFTER PRESCALE

#define PWM_OVF_FREQ (PWM_CLK_HZ/256UL) //gives the pwm timer overflow freq


//equation for finding increment value for the 16-bit accumulator
// increment = ((2^16)*(256)*Fout)/ F_PWM = 1.048576 * Fout 

//#define FREQ_TO_INC16(f) ((((1<<16)<<8)*f)/PWM_CLK_HZ)


//this table is for a 16MHz PWM clk.
//goes from C0 to G9 MIDI range, but index in array is (MIDI num - 12)
const uint16_t inc16_note_vals[] PROGMEM = {
    //these first few note's freqs are from here:
    // http://www.phy.mtu.edu/~suits/notefreqs.html
    17,     //C0 16.35Hz 
    18,     //C#0 17.32Hz
    19,     //D0 18.35Hz
    20,     //D#0 19.45Hz
    22,     //E0 20.60Hz
    23,     //F0 21.83Hz
    24,     //F#0 23.12Hz
    26,     //G0 24.50Hz
    27,     //G#0 25.96Hz
            //the next ones are found in "../files/pitch-freq.txt" file
            //from which was sourced from this web page:
            // http://people.virginia.edu/~pdr4h/pitch-freq.html
    29,     //A0 27.5Hz //end of piano low range
    31,     //A#0
    32,     //B0
    
    34,     //C1
    36,     //C#1
    38,     //D1
    41,     //D#1
    43,     //E1
    46,     //F1
    48,     //F#1
    51,     //G1
    54,     //G#1
    58,     //A1 55Hz
    61,     //A#1
    65,     //B1
    
    69,     //C2 65.406Hz
    73,     //C#2
    77,     //D2
    82,     //D#2
    86,     //E2
    92,     //F2
    97,     //F#2
    103,    //G2
    109,    //G#2
    115,    //A2
    122,    //A#2
    129,    //B2
    
    137,    //C3
    145,    //C#3
    154,    //D3
    163,    //D#3
    173,    //E3
    183,    //F3
    194,    //F#3
    206,    //G3
    218,    //G#3
    231,    //A3
    244,    //A#3
    259,    //B3
    
    274,    //C4 261.626Hz MIDI 60 "Middle C"
    291,    //C#4
    308,    //D4
    326,    //D#4
    346,    //E4
    366,    //F4
    388,    //F#4
    411,    //G4
    435,    //G#4
    461,    //A4 440Hz
    489,    //A#4
    518,    //B4
    
    549,    //C5 523.251Hz
    581,    //C#5
    616,    //D5
    652,    //D#5
    691,    //E5
    732,    //F5
    776,    //F#5
    822,    //G5
    871,    //G#5
    923,    //A5 880Hz
    978,    //A#5
    1036,   //B5
    
    1097,   //C6 1046.502 MIDI 84
    1163,   //C#6
    1232,   //D6
    1305,   //D#6
    1383,   //E6
    1465,   //F6
    1552,   //F#6
    1644,   //G6
    1742,   //G#6
    1845,   //A6 1760Hz 
    1955,   //A#6
    2071,   //B6
    
    2194,   //C7 2093.005Hz MIDI 96
    

    
} 
